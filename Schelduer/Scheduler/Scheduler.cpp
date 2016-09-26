#include "Scheduler.h"

Scheduler::Scheduler() : wskConnectionManager(nullptr),
                         firstJobStarted(false)
{
    Traces() << "\n" << "LOG: Scheduler::Scheduler()";
    condition_var = new std::condition_variable();
    boardsToAnalyse.SetCondVar(condition_var);

}

ConnectionManager * Scheduler::GetConnectionManager()
{
    Traces() << "\n" << "LOG: ConnectionManager * Scheduler::GetConnectionManager()";

    std::lock_guard<std::mutex> ls(mutex);
    return wskConnectionManager;
}

void Scheduler::SetConnectionManager(ConnectionManager *wsk)
{
    Traces() << "\n" << "LOG: void Scheduler::SetConnectionManager(ConnectionManager *wsk)";

    std::lock_guard<std::mutex> ls(mutex);
    wskConnectionManager = wsk;
    wskConnectionManager->SetConditionVariable(condition_var);
}

void Scheduler::Start(const unsigned short numofthread)
{
    Traces() << "\n" << "LOG: void Scheduler::Start(const unsigned short numofthread)";

    if (numofthread == 0)
    {
        Traces() << "\n" << "ERROR: Number of threads can not be 0!";
        throw std::string("Wrong number of threads");
    }

    for (unsigned short i=0;i<numofthread;i++)
    {
        Traces() << "\n" << "LOG: Start thread: " << i + 1;
        schedulerThread[i] = std::move(std::thread(&Scheduler::StartScheduling,
                                            this));

        schedulerThread[i].detach();
    }
}

void Scheduler::StartScheduling()
{
    Traces() << "\n" << "LOG: void Scheduler::StartScheduling()";

    Message tmpMessage;
    char *dest = new char[MessageCoder::MaxMessageSize()];
    bool wasMessage;
    bool isNewBoardToAnalyse;
    bool isNewMessage;

    while (true)
    {
        Traces() << "\n" << "LOG: Waiting for a job..";
        std::mutex tmpMutex;
        std::unique_lock<std::mutex> guard(tmpMutex);

        condition_var->wait(guard,[this, &isNewBoardToAnalyse, &isNewMessage]
        {
            isNewBoardToAnalyse = (!boardsToAnalyse.Empty()) & (!freeWorkers.Empty());
            isNewMessage = wskConnectionManager->IsNewMessage();
            return (isNewBoardToAnalyse | isNewMessage); }
        );

        if (isNewMessage)
        {
            Traces() << "\n" << "LOG: Try reading message from message queue";

            try
            {
                wasMessage = true;
                tmpMessage = wskConnectionManager->GetFirstMessage();
            }
            catch (std::string)
            {
                wasMessage = false;
                Traces() << "\n" << "LOG: List empty. Not a bug.";
            }

            if (wasMessage)
            {
                std::map<std::string, std::string> messageContent;
                MessageCoder::MessageToMap(tmpMessage.GetWskMessage(), messageContent);

                MessageInterpreting(tmpMessage.GetTCPConnection_ptr(), messageContent, dest);
            }

            tmpMessage.Clear();
        } else
        if (isNewBoardToAnalyse)
        {
            Traces() << "\n" << "LOG: New board to analyse";            
            DistributeWorkToWorkers(dest);
        }
    }

    delete [] dest;
}

void Scheduler::MessageInterpreting(TCPConnection_ptr socket, std::map<std::string, std::string> & data, char * dest)
{
    Traces() << "\n" << "LOG: void Scheduler::MessageInterpreting(TCPConnection_ptr socket, std::map<std::string, std::string> & dest, char * data, char * dest)";

    try
    {
        std::string action = data.at(MessageCoder::ACTION);

        if (action == MessageCoder::CLOSE_CNNECTION)
        {
            Traces() << "\n" << "LOG: action == MessageCoder::CLOSE_CNNECTION";

            bool removeResult;

            removeResult = RemoveClient(socket);
            if (!removeResult) removeResult = RemoveWorker(socket);

        } else
        if (action == MessageCoder::SET_STATE)
        {
            Traces() << "\n" << "LOG: action == MessageCoder::SET_STATE";

            SetState(socket, data, dest);
        } else            
        if (action == MessageCoder::SET_ROLE)
        {
            Traces() << "\n" << "LOG: action == MessageCoder::SET_ROLE";            
            SetRole(socket, data, dest);            
        } else
        if (action == MessageCoder::GET_SERVER_STATE)
        {
            Traces() << "\n" << "LOG: action == MessageCoder::GET_SERVER_STATE";
            SendServerState(socket, state, data, dest);
        } else
        if (action == MessageCoder::START_WORK)
        {
            Traces() << "\n" << "LOG: action == MessageCoder::START_WORK";

            Board tmpBoard;
            MessageCoder::MapToBoard(data, &tmpBoard);
            state.SetBoard(tmpBoard);
            state.SetThinking(true);
            boardsToAnalyse.PushBack(tmpBoard);
            SendServerState(socket, state, data, dest);
            clients.At(socket)->SetConnectionState(Client::ConnectionState::WaitForOkMessageAfterSendStatus);
            CreateTimeoutGuard(socket, 5000);            
        } else
        if (action == MessageCoder::OK)
        {
            Traces() << "\n" << "LOG: action == MessageCoder::OK";

            bool okFlag = false;

            try
            {
                Client_ptr tmpClinet = clients.At(socket);

                Traces() << "\n" << "LOG: Client found on the tiemr list";

                if (tmpClinet->GetConnectionState() == Client::ConnectionState::WaitForOkMessageAfterSendStatus)
                {
                    timerList.RemoveFromList(socket);
                    tmpClinet->SetConnectionState(Client::ConnectionState::None);
                } else
                {
                    Traces() << "\n" << "ERR: Unexpected OK message from client";
                }

                okFlag = true;
            }
            catch (const std::out_of_range& oor)
            {

            }

            if (!okFlag)
            {
                try
                {
                    Worker_ptr tmpWorker = workers.At(socket);

                    Traces() << "\n" << "LOG: Worker found on the tiemr list";

                    if (tmpWorker->GetConnectionState() == Worker::ConnectionState::WaitForOkMessageAfterSendJob)
                    {
                        timerList.RemoveFromList(socket);
                        tmpWorker->SetConnectionState(Worker::ConnectionState::None);
                        tmpWorker->SetState(Peers::BUSY);
                    } else
                    {
                        Traces() << "\n" << "ERR: Unexpected OK message from worker";
                    }

                    okFlag = true;
                }
                catch (const std::out_of_range& oor)
                {

                }
            }

        } else
        if (action == MessageCoder::TIMEOUT)
        {
            Traces() << "\n" << "LOG: action == MessageCoder::TIMEOUT";

            try
            {
                clients.At(socket);
                Traces() << "\n" << "LOG: Client found on the tiemr list";
                socket->Close();
            }
            catch (const std::out_of_range& oor)
            {

            }

            try
            {
                Worker_ptr tmpWorker = workers.At(socket);
                Traces() << "\n" << "LOG: Client found on the tiemr list";

                if (tmpWorker->GetConnectionState() == Worker::ConnectionState::WaitForOkMessageAfterSendJob)
                {
                    socket->Close();
                    //boardsToAnalyse.PushBack(state.GetBoard());

                }


            }
            catch (const std::out_of_range& oor)
            {

            }
        }
        else
        {
            Traces() << "\n" << "ERR: Unexpected action: " << action << " from" << socket->GetIp() << ":" << socket->GetPort();
        }

    }
    catch (std::out_of_range)
    {
        Traces() << "\n" << "ERR: Protocol error host: " << socket->GetIp() << ":" << socket->GetPort();
    }
}

void Scheduler::SetRole(TCPConnection_ptr socket, const std::map<std::string, std::string> & data, char * dest)
{
    Traces() << "\n" << "LOG: void Scheduler::SetRole(TCPConnection_ptr socket, std::map<std::string, std::string> & dest)";
    try
    {
        std::string role = data.at(MessageCoder::ROLE);

        if (atoi(role.c_str()) == MessageCoder::ROLE_ENUM::CLIENT)
        {            
            AddClient(socket, data, dest);
        } else
        if (atoi(role.c_str()) == MessageCoder::ROLE_ENUM::WORKER)
        {\
            AddWorker(socket, data, dest);            
        }
    }
    catch (std::out_of_range)
    {
        Traces() << "\n" << "ERR: Protocol error host: " << socket->GetIp() << ":" << socket->GetPort();
    }
}

void Scheduler::SetState(TCPConnection_ptr socket, const std::map<std::string, std::string> & data, char * dest)
{
    Traces() << "\n" << "LOG: void Scheduler::SetState(TCPConnection_ptr socket, std::map<std::string, std::string> & dest)";
    try
    {
       Worker_ptr tmpWorker = workers.At(socket);
       tmpWorker->SetMaxThread(std::stoi(data.at(MessageCoder::NUM_OF_THREAD)));
       tmpWorker->SetState(static_cast<Peers::STATE>(std::stoi(data.at(MessageCoder::STATE))));

       UpdateFreeWorkerList(socket, tmpWorker);

       std::string messageId = data.at(MessageCoder::MESSAGE_ID);

       MessageCoder::ClearChar(dest, MessageCoder::MaxMessageSize());

       MessageCoder::CreateOkMessage(messageId, dest);

       Traces() << "\n" << "LOG: Sending: " << dest;

       socket->SendMessage(dest);
    }
    catch (std::out_of_range)
    {
        Traces() << "\n" << "ERR: Protocol error host: " << socket->GetIp() << ":" << socket->GetPort();
    }
}

void Scheduler::SendServerState(TCPConnection_ptr socket, const ServerState & serverState, const std::map<std::string, std::string> & data, char * dest)
{
    Traces() << "\n" << "void Scheduler::SendState(TCPConnection_ptr socket, const ServerState & serverState, const std::map<std::string, std::string> & data, char * dest)";
    try
    {
        std::string messageId = data.at(MessageCoder::MESSAGE_ID);

        MessageCoder::ClearChar(dest, MessageCoder::MaxMessageSize());
        MessageCoder::CreateServerStateMessage(serverState, messageId, dest);

        Traces() << "\n" << "LOG: Sending: " << dest;

        socket->SendMessage(dest);
    }
    catch (std::out_of_range)
    {
        Traces() << "\n" << "ERR: Protocol error host: " << socket->GetIp() << ":" << socket->GetPort();
    }
}

void Scheduler::AddClient(TCPConnection_ptr socket, const std::map<std::string, std::string> & data, char * dest)
{
    Traces() << "\n" << "LOG: void Scheduler::AddClient(TCPConnection_ptr socket, const std::map<std::string, std::string> & data, char * dest)";


    if (clients.Insert(socket, Client_ptr(new Client()))  == true)
    {
        Traces() << "\n" << "ERR: Element already existed!";
    } else
    {
        std::string messageId = data.at(MessageCoder::MESSAGE_ID);

        MessageCoder::ClearChar(dest, MessageCoder::MaxMessageSize());

        MessageCoder::CreateOkMessage(messageId, dest);

        Traces() << "\n" << "LOG: Sending: " << dest;

        socket->SendMessage(dest);
    }
}

void Scheduler::AddWorker(TCPConnection_ptr socket, const std::map<std::string, std::string> & data, char * dest)
{
    Traces() << "\n" << "LOG: void Scheduler::AddWorker(TCPConnection_ptr socket, const std::map<std::string, std::string> & data, char * dest)";

    Worker_ptr tmpWorkerPtr;
    tmpWorkerPtr.reset(new Worker());

    if (workers.Insert(socket, tmpWorkerPtr)  == true)
    {
        Traces() << "\n" << "ERR: Element already existed!";
    } else
    {        
        std::string messageId = data.at(MessageCoder::MESSAGE_ID);
        MessageCoder::ClearChar(dest, MessageCoder::MaxMessageSize());
        MessageCoder::CreateOkMessage(messageId, dest);

        Traces() << "\n" << "LOG: Sending: " << dest;                
        socket->SendMessage(dest);
    }    
}

bool Scheduler::RemoveClient(TCPConnection_ptr socket)
{
    Traces() << "\n" << "LOG: void Scheduler::RemoveClient(TCPConnection_ptr socket)";

    bool flag =  false;

    try
    {
        clients.At(socket);
        clients.Erase(socket);
        wskConnectionManager->CloseConnection(socket);
    }
    catch (const std::out_of_range& oor)
    {
        flag = true;
    }

    if (!flag) Traces() << "\n" << "LOG: Client removed";

    return !flag;
}

bool Scheduler::RemoveWorker(TCPConnection_ptr socket)
{
    Traces() << "\n" << "LOG: void Scheduler::RemoveSocket(TCPConnection_ptr socket)";

    bool flag =  false;    

    try
    {        
        UpdateFreeWorkerList(socket, workers.At(socket));
        workers.Erase(socket);
        wskConnectionManager->CloseConnection(socket);
    }
    catch (const std::out_of_range& oor)
    {
        flag = true;
    }

    if (!flag)
    {
        Traces() << "\n" << "LOG: Worker removed";
    }

    return !flag;
}

void Scheduler::CreateTimeoutGuard(TCPConnection_ptr socket, const unsigned int miliseconds)
{
    Traces() << "\n" << "LOG: void Scheduler::CreateTimeoutGuard(TCPConnection_ptr socket, const unsigned int miliseconds)";

    QueueTimer_ptr tmpTimer;
    tmpTimer = std::shared_ptr<QueueTimer> (new QueueTimer());
    tmpTimer->SetTime(5000);
    tmpTimer->SetQueue(wskConnectionManager->GetMessageQueue());
    char *tmpData = new char[MessageCoder::MaxMessageTimeoutSize()];
    MessageCoder::ClearChar(tmpData, MessageCoder::MaxMessageTimeoutSize());
    MessageCoder::CreateTimeoutMessage(tmpData);
    Message tmpMessage;
    tmpMessage.CopyWsk(socket, tmpData);
    tmpTimer->SetMessageToSend(tmpMessage);
    tmpTimer->Start();
    timerList.InsertIntoList(socket, tmpTimer);
}

void Scheduler::UpdateFreeWorkerList(TCPConnection_ptr & socket, Worker_ptr worker)
{
    Traces() << "\n" << "LOG: void Scheduler::UpdateFreeWorkerList(const Worker & worker)";

    if (worker->GetState() == Peers::FREE)
    {
        Traces() << "\n" << "LOG: Free workers added to free workers list";

        freeWorkers.PushBack(socket);
    } else
    {
        Traces() << "\n" << "LOG: Free workers removed from free workers list";

        freeWorkers.Remove(socket);
    }
}

void Scheduler::DistributeWorkToWorkers(char * dest)
{
    Traces() << "\n" << "LOG: void Scheduler::DistributeWorkToWorkers()";

    if (freeWorkers.Size() == 1)
    {
        Traces() << "\n" << "LOG: freeWorkers.Size() == 1";

        Traces() << "\n" << "LOG: freeWorkers.Size() >  1";

        bool listEmpty = false;
        TCPConnection_ptr tmpWorkerSocket;
        Worker_ptr tmpWorker;

        try
        {
            tmpWorkerSocket = freeWorkers.PopFront();
        }
        catch (...)
        {
            listEmpty = true;
        }

        if (!listEmpty)
        {
            try
            {
                tmpWorker = workers.At(tmpWorkerSocket);
                tmpWorker->SetState(Peers::STATE::BUSY);
            }
            catch (...)
            {
                listEmpty = true;
            }

            if (!listEmpty)
            {
                if (!firstJobStarted)
                {
                    Traces() << "\n" << "LOG: Send job and order worker to return N-result fast";

                    std::string messageId = MessageCoder::CreateMessageId();
                    std::string jobId = MessageCoder::CreateMessageId();
                    MessageCoder::ClearChar(dest, MessageCoder::MaxMessageSize());



                    MessageCoder::CreateStartAnalyseWork(state.GetMaxTime(),
                                                         boardsToAnalyse.PopFront(),
                                                         messageId,
                                                         jobId,
                                                         dest);

                    tmpWorker->SetConnectionState(Worker::ConnectionState::WaitForOkMessageAfterSendJob);
                    tmpWorkerSocket->SendMessage(dest);
                    firstJobStarted = true;
                    CreateTimeoutGuard(tmpWorkerSocket, 5000);
                } else
                {
                    Traces() << "\n" << "LOG: Send job to worker";
                }
            }
        }

    } else
    {
        Traces() << "\n" << "LOG: freeWorkers.Size() >  1";

        bool listEmpty = false;
        TCPConnection_ptr tmpWorkerSocket;
        Worker_ptr tmpWorker;

        try
        {
            tmpWorkerSocket = freeWorkers.PopFront();
        }
        catch (...)
        {
            listEmpty = true;
        }

        if (!listEmpty)
        {
            try
            {
                tmpWorker = workers.At(tmpWorkerSocket);
                tmpWorker->SetState(Peers::STATE::BUSY);
            }
            catch (...)
            {
                listEmpty = true;
            }

            if (!listEmpty)
            {
                if (!firstJobStarted)
                {
                    Traces() << "\n" << "LOG: Send job and order worker to return N-result fast";

                    std::string messageId = MessageCoder::CreateMessageId();
                    std::string jobId = MessageCoder::CreateMessageId();
                    MessageCoder::ClearChar(dest, MessageCoder::MaxMessageSize());



                    MessageCoder::CreateStartAnalyseWorkAndReturnNResultFast(state.GetMaxTime(),
                                                                             freeWorkers.Size(),
                                                                             boardsToAnalyse.PopFront(),
                                                                             messageId,
                                                                             jobId,
                                                                             dest);

                    tmpWorker->SetConnectionState(Worker::ConnectionState::WaitForOkMessageAfterSendJob);
                    tmpWorkerSocket->SendMessage(dest);                    
                    firstJobStarted = true;
                    CreateTimeoutGuard(tmpWorkerSocket, 5000);
                } else
                {
                    Traces() << "\n" << "LOG: Send job to worker";
                }
            }
        }
    }
}

Scheduler::~Scheduler()
{
    delete condition_var;
}
