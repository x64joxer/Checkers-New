#include "Scheduler.h"

Scheduler::Scheduler() : wskConnectionManager(nullptr),
                         firstJobStarted(false),
                         workOngoing(false)
{
    TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: Scheduler::Scheduler()";
    condition_var = new std::condition_variable();
    boardsToAnalyse.SetCondVar(condition_var);

}

ConnectionManager * Scheduler::GetConnectionManager()
{
    TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: ConnectionManager * Scheduler::GetConnectionManager()";

    std::lock_guard<std::mutex> ls(mutex);
    return wskConnectionManager;
}

void Scheduler::SetConnectionManager(ConnectionManager *wsk)
{
    TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: void Scheduler::SetConnectionManager(ConnectionManager *wsk)";

    std::lock_guard<std::mutex> ls(mutex);
    wskConnectionManager = wsk;
    wskConnectionManager->SetConditionVariable(condition_var);
}

void Scheduler::Start(const unsigned short numofthread)
{
    TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: void Scheduler::Start(const unsigned short numofthread)";

    if (numofthread == 0)
    {
        Traces() << "\n" << "ERROR: Number of threads can not be 0!";
        throw std::string("Wrong number of threads");
    }

    for (unsigned short i=0;i<numofthread;i++)
    {
        TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: Start thread: " << i + 1;
        schedulerThread[i] = std::move(std::thread(&Scheduler::StartScheduling,
                                            this));

        schedulerThread[i].detach();
    }
}

void Scheduler::StartScheduling()
{
    TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: void Scheduler::StartScheduling()";

    Message tmpMessage;
    TCPConnection_ptr tmpTCP_Connection_ptr;
    char *dest = new char[MessageCoder::MaxMessageSize()];
    bool wasMessage;
    bool isNewBoardToAnalyse;
    bool isNewMessage;
    bool isClientToUpate;

    while (true)
    {
        TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: Waiting for a job..";
        std::mutex tmpMutex;
        std::unique_lock<std::mutex> guard(tmpMutex);

        condition_var->wait(guard,[this, &isNewBoardToAnalyse, &isNewMessage, &isClientToUpate]
        {
            isNewBoardToAnalyse = (!boardsToAnalyse.Empty()) & (!freeWorkers.Empty()) & (workOngoing);
            if (Traces::GetMilisecondsSinceEpoch() > ((state.GetStartTime() + state.GetMaxTime()) - (ProgramVariables::GetTimeReserveToSendBestResultToClient() + ProgramVariables::GetTimeToSendJobsToFreeWorkers()))) isNewBoardToAnalyse = false;
            isNewMessage = wskConnectionManager->IsNewMessage();
            isClientToUpate = !clientsToStateUpdate.Empty();

            return (isNewBoardToAnalyse | isNewMessage | isClientToUpate); }
        );

        if (isNewMessage)
        {
            TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: Try reading message from message queue";

            try
            {
                wasMessage = true;
                tmpMessage = wskConnectionManager->GetFirstMessage();
            }
            catch (...)
            {
                wasMessage = false;
                TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: List empty. Not a bug.";
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
            TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: New board to analyse";
            DistributeWorkToWorkers(dest);
        }
        if (isClientToUpate)
        {
            TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: Client to update";

            UpdateNextClientStatus(tmpTCP_Connection_ptr, dest);
        }
    }

    delete [] dest;
}

void Scheduler::MessageInterpreting(TCPConnection_ptr socket, std::map<std::string, std::string> & data, char * dest)
{
    TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: void Scheduler::MessageInterpreting(TCPConnection_ptr socket, std::map<std::string, std::string> & dest, char * data, char * dest)";

    try
    {
        std::string action = data.at(MessageCoder::ACTION);

        if (action == MessageCoder::CLOSE_CNNECTION)
        {
            TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: action == MessageCoder::CLOSE_CNNECTION";

            bool removeResult;

            removeResult = RemoveClient(socket);
            if (!removeResult) removeResult = RemoveWorker(socket);

        } else
        if (action == MessageCoder::SET_STATE)
        {
            TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: action == MessageCoder::SET_STATE";

            SetState(socket, data, dest);
        } else            
        if (action == MessageCoder::SET_ROLE)
        {
            TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: action == MessageCoder::SET_ROLE";
            SetRole(socket, data, dest);            
        } else
        if (action == MessageCoder::GET_SERVER_STATE)
        {
            TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: action == MessageCoder::GET_SERVER_STATE";
            SendServerState(socket, state, data, dest);
        } else
        if (action == MessageCoder::START_WORK)
        {
            TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: action == MessageCoder::START_WORK";

            workOngoing = true;

            Board tmpBoard;
            tmpBoard = state.GetBoard();
            state.SetPreviousBoard(tmpBoard);

            MessageCoder::MapToBoard(data, &tmpBoard);
            state.SetBoard(tmpBoard);
            state.SetThinking(true);
            state.SetMaxTime(std::atoi(data.at(MessageCoder::MAX_TIME).c_str()) - ProgramVariables::GetTimeReserveToSendBestResultToClient());
            state.SetStartTime(Traces::GetMilisecondsSinceEpoch());
            CreateTimeToSendResultToClientsGuard(socket, state.GetMaxTime());

            boardsToAnalyse.Clear();
            boardsToAnalyse.PushBack(tmpBoard);

            SendStateToAllClients(data, dest);
        } else
        if (action == MessageCoder::BEST_RESULT)
        {
            TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: action == MessageCoder::BEST_RESULT";

            RecevieBestResult(socket, data, dest);
        } else
        if (action == MessageCoder::TIME_TO_SEND_RESULT_TO_CLIENTS)
        {
            TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: action == MessageCoder::TIME_TO_SEND_RESULT_TO_CLIENTS";

            FinishWork(data, dest);
        } else
        if (action == MessageCoder::OK)
        {
            TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: action == MessageCoder::OK";

            bool okFlag = false;

            try
            {
                Client_ptr tmpClinet = clients.At(socket);

                TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: Client found on the tiemr list";

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

                    TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: Worker found on the tiemr list";

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
            TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: action == MessageCoder::TIMEOUT";

            timerList.RemoveFromList(socket);

            try
            {                
                clients.At(socket);
                TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: Client found on the tiemr list";
                socket->Close();
            }
            catch (const std::out_of_range& oor)
            {

            }

            try
            {
                Worker_ptr tmpWorker = workers.At(socket);
                TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: Client found on the tiemr list";

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
    TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: void Scheduler::SetRole(TCPConnection_ptr socket, std::map<std::string, std::string> & dest)";
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
    TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: void Scheduler::SetState(TCPConnection_ptr socket, std::map<std::string, std::string> & dest)";
    try
    {
       Worker_ptr tmpWorker = workers.At(socket);
       tmpWorker->SetMaxThread(std::stoi(data.at(MessageCoder::NUM_OF_THREAD)));
       tmpWorker->SetState(static_cast<Peers::STATE>(std::stoi(data.at(MessageCoder::STATE))));

       UpdateFreeWorkerList(socket, tmpWorker);

       std::string messageId = data.at(MessageCoder::MESSAGE_ID);

       MessageCoder::ClearChar(dest, MessageCoder::MaxMessageSize());

       MessageCoder::CreateOkMessage(messageId, dest);

       TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: Sending: " << dest;

       socket->SendMessage(dest);
    }
    catch (std::out_of_range)
    {
        Traces() << "\n" << "ERR: Protocol error host: " << socket->GetIp() << ":" << socket->GetPort();
    }
}

void Scheduler::SendServerState(TCPConnection_ptr socket, const ServerState & serverState, const std::string & messageId, char * dest)
{
    Traces() << "\n" << "TCPConnection_ptr socket, const ServerState & serverState, const std::string & messageId, char * dest";
    try
    {
        MessageCoder::ClearChar(dest, MessageCoder::MaxMessageSize());
        MessageCoder::CreateServerStateMessage(serverState, messageId, dest);

        TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: Sending: " << dest;

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

        TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: Sending: " << dest;

        socket->SendMessage(dest);
    }
    catch (std::out_of_range)
    {
        Traces() << "\n" << "ERR: Protocol error host: " << socket->GetIp() << ":" << socket->GetPort();
    }
}

void Scheduler::AddClient(TCPConnection_ptr socket, const std::map<std::string, std::string> & data, char * dest)
{
    TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: void Scheduler::AddClient(TCPConnection_ptr socket, const std::map<std::string, std::string> & data, char * dest)";


    if (clients.Insert(socket, Client_ptr(new Client()))  == true)
    {
        Traces() << "\n" << "ERR: Element already existed!";
    } else
    {
        std::string messageId = data.at(MessageCoder::MESSAGE_ID);

        MessageCoder::ClearChar(dest, MessageCoder::MaxMessageSize());

        MessageCoder::CreateOkMessage(messageId, dest);

        TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: Sending: " << dest;

        socket->SendMessage(dest);
    }
}

void Scheduler::AddWorker(TCPConnection_ptr socket, const std::map<std::string, std::string> & data, char * dest)
{
    TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: void Scheduler::AddWorker(TCPConnection_ptr socket, const std::map<std::string, std::string> & data, char * dest)";

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

        TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: Sending: " << dest;
        socket->SendMessage(dest);
    }    
}

bool Scheduler::RemoveClient(TCPConnection_ptr socket)
{
    TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: void Scheduler::RemoveClient(TCPConnection_ptr socket)";

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

    if (!flag) TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: Client removed";

    return !flag;
}

bool Scheduler::RemoveWorker(TCPConnection_ptr socket)
{
    TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: void Scheduler::RemoveSocket(TCPConnection_ptr socket)";

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
        TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: Worker removed";
    }

    return !flag;
}

void Scheduler::CreateTimeoutGuard(TCPConnection_ptr socket, const unsigned int miliseconds)
{
    TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: void Scheduler::CreateTimeoutGuard(TCPConnection_ptr socket, const unsigned int miliseconds)";

    QueueTimer_ptr tmpTimer;
    tmpTimer = std::shared_ptr<QueueTimer> (new QueueTimer());
    tmpTimer->SetTime(miliseconds);
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

void Scheduler::CreateTimeToSendResultToClientsGuard(TCPConnection_ptr socket, const unsigned int miliseconds)
{
    TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: void Scheduler::CreateTimeToSendResultToClientsGuard(TCPConnection_ptr socket, const unsigned int miliseconds)";

    QueueTimer_ptr tmpTimer;
    tmpTimer = std::shared_ptr<QueueTimer> (new QueueTimer());
    tmpTimer->SetTime(miliseconds);
    tmpTimer->SetQueue(wskConnectionManager->GetMessageQueue());
    char *tmpData = new char[MessageCoder::MaxMessageTimeToSendResultToClientsSize()];
    MessageCoder::ClearChar(tmpData, MessageCoder::MaxMessageTimeToSendResultToClientsSize());
    MessageCoder::CreateTimeToSendResultToClientsMessage(tmpData);
    Message tmpMessage;
    tmpMessage.CopyWsk(nullptr, tmpData);
    tmpTimer->SetMessageToSend(tmpMessage);
    tmpTimer->Start();
    timerList.InsertIntoList(nullptr, tmpTimer);
}

void Scheduler::UpdateFreeWorkerList(TCPConnection_ptr & socket, Worker_ptr worker)
{
    TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: void Scheduler::UpdateFreeWorkerList(const Worker & worker)";

    if (worker->GetState() == Peers::FREE)
    {
        TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: Free workers added to free workers list";

        freeWorkers.PushBack(socket);
    } else
    {
        TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: Free workers removed from free workers list";

        freeWorkers.Remove(socket);
    }
}

void Scheduler::DistributeWorkToWorkers(char * dest)
{
    TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: void Scheduler::DistributeWorkToWorkers()";

    bool listEmpty = false;
    TCPConnection_ptr tmpWorkerSocket;
    Worker_ptr tmpWorker;
    bool tmpFirstJobStarted;
    bool tmpSizeMoreThanOne;
    Board tmpBoard;
    unsigned int tmpFreeWorkerListSize;

    {
        std::lock_guard<std::mutex> ls(mutexWorkDistribute);

        //Get free worker socket;
        try
        {
            tmpWorkerSocket = freeWorkers.PopFront();
        }
        catch (...)
        {
            listEmpty = true;
        }

        //Get  free worker
        if (!listEmpty)
        {
            try
            {
                tmpWorker = workers.At(tmpWorkerSocket);
                tmpWorker->SetState(Peers::STATE::BUSY);
                listEmpty = false;
            }
            catch (...)
            {
                listEmpty = true;
            }
        } else
        {
            freeWorkers.PushBack(tmpWorkerSocket);
        }

        //Get  board to analyse
        if (!listEmpty)
        {
            try
            {
                tmpBoard = boardsToAnalyse.PopFront();
                listEmpty = false;
            }
            catch (...)
            {
                listEmpty = true;
            }
        } else
        {
            boardsToAnalyse.PushBack(tmpBoard);
        }

        if (!listEmpty)
        {
             tmpFirstJobStarted = firstJobStarted;

             if (!firstJobStarted)
             {
                 firstJobStarted = true;
                 tmpFreeWorkerListSize = freeWorkers.Size() + 1;

                 if (tmpFreeWorkerListSize == 1)
                 {
                    tmpSizeMoreThanOne = false;
                 } else
                 {
                    tmpSizeMoreThanOne = true;
                 }
             }
        }
    }


    if (!listEmpty)
    {
        if ((!tmpFirstJobStarted) && (!tmpSizeMoreThanOne))
        {
            TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: Send job to only one worker";

            std::string messageId = MessageCoder::CreateMessageId();
            std::string jobId = MessageCoder::CreateMessageId();
            MessageCoder::ClearChar(dest, MessageCoder::MaxMessageSize());



            MessageCoder::CreateStartAnalyseWork(state.GetMaxTime() - (Traces::GetMilisecondsSinceEpoch() - state.GetStartTime()),
                                                 tmpBoard,
                                                 messageId,
                                                 jobId,
                                                 true,
                                                 dest);

            tmpWorker->SetConnectionState(Worker::ConnectionState::WaitForOkMessageAfterSendJob);
            tmpWorkerSocket->SendMessage(dest);
            firstJobStarted = true;
            CreateTimeoutGuard(tmpWorkerSocket, ProgramVariables::GetMaxTimeoutForMessageResponse());
        } else
        if ((!tmpFirstJobStarted) && (tmpSizeMoreThanOne))
        {
            TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: Send job and order worker to return N-result fast";

            std::string messageId = MessageCoder::CreateMessageId();
            std::string jobId = MessageCoder::CreateMessageId();
            MessageCoder::ClearChar(dest, MessageCoder::MaxMessageSize());



            MessageCoder::CreateStartAnalyseWorkAndReturnNResultFast(state.GetMaxTime() - (Traces::GetMilisecondsSinceEpoch() - state.GetStartTime()),
                                                                     tmpFreeWorkerListSize - 1,
                                                                     tmpBoard,
                                                                     messageId,
                                                                     jobId,
                                                                     dest);

            tmpWorker->SetConnectionState(Worker::ConnectionState::WaitForOkMessageAfterSendJob);
            tmpWorkerSocket->SendMessage(dest);
            firstJobStarted = true;
            CreateTimeoutGuard(tmpWorkerSocket, ProgramVariables::GetMaxTimeoutForMessageResponse());

        } else
        if (tmpFirstJobStarted)
        {
            TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: Send job to worker";

            std::string messageId = MessageCoder::CreateMessageId();
            std::string jobId = MessageCoder::CreateMessageId();
            MessageCoder::ClearChar(dest, MessageCoder::MaxMessageSize());



            MessageCoder::CreateStartAnalyseWork(state.GetMaxTime() - (Traces::GetMilisecondsSinceEpoch() - state.GetStartTime()),
                                                 tmpBoard,
                                                 messageId,
                                                 jobId,
                                                 false,
                                                 dest);

            tmpWorker->SetConnectionState(Worker::ConnectionState::WaitForOkMessageAfterSendJob);
            tmpWorkerSocket->SendMessage(dest);
            CreateTimeoutGuard(tmpWorkerSocket, ProgramVariables::GetMaxTimeoutForMessageResponse());
        }
    }

}

void Scheduler::UpdateNextClientStatus(TCPConnection_ptr tmpTCP_Connection_ptr, char * dest)
{
    TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: void Scheduler::UpdateNextClientStatus(TCPConnection_ptr tmpTCP_Connection_ptr, char * dest)";

    bool wasClientToUpdate = true;

    try
    {
        tmpTCP_Connection_ptr = clientsToStateUpdate.PopFront();
    }
    catch (std::string)
    {
        wasClientToUpdate = false;
        TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: List empty. Not a bug.";
    }

    if (wasClientToUpdate)
    {
           SendServerState(tmpTCP_Connection_ptr, state, MessageCoder::CreateMessageId(), dest);
           clients.At(tmpTCP_Connection_ptr)->SetConnectionState(Client::ConnectionState::WaitForOkMessageAfterSendStatus);
           CreateTimeoutGuard(tmpTCP_Connection_ptr, ProgramVariables::GetMaxTimeoutForMessageResponse());
    }
}

void Scheduler::RecevieBestResult(TCPConnection_ptr socket, const std::map<std::string, std::string> & data, char * dest)
{
    TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: void Scheduler::RecevieBestResult(const std::map<std::string, std::string> & data)";

    Board tmpBoard;
    MessageCoder::MapToBoard(data, &tmpBoard);
    boardsToAnalyse.PushBack(tmpBoard);

    std::string messageId = data.at(MessageCoder::MESSAGE_ID);
    MessageCoder::ClearChar(dest, MessageCoder::MaxMessageSize());
    MessageCoder::CreateOkMessage(messageId, dest);

    TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: Sending: " << dest;
    socket->SendMessage(dest);
}

void Scheduler::FinishWork(const std::map<std::string, std::string> & data, char * dest)
{
    TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: void Scheduler::FinishWork(const std::map<std::string, std::string> & data, char * dest)";

    timerList.RemoveFromList(nullptr);
    workOngoing = false;
    firstJobStarted = false;
    state.SetBoard(CalculateBestResult());
    state.SetThinking(false);
    SendStateToAllClients(data, dest);
}

void Scheduler::SendStateToAllClients(const std::map<std::string, std::string> & data, char * dest)
{
    TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: void Scheduler::SendStateToAllClients(const std::map<std::string, std::string> & data, char * dest)";

    clients.GetAllKeys(clientsToStateUpdate);
}

Board Scheduler::CalculateBestResult()
{
    TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: Board Scheduler::CalculateBestResult()";

    unsigned int listSize = boardsToAnalyse.Size();

    if (listSize == 1)
    {
        return boardsToAnalyse.PopFront().GetOrigin();
    } else
    if (listSize > 1)
    {
        Board bestResult;

        try
        {
            bestResult = boardsToAnalyse.PopFront();

            Board tmpBoard;

            while(!boardsToAnalyse.Empty())
            {
                try
                {
                    tmpBoard = boardsToAnalyse.PopFront();
                    if (bestResult.GetResult() < tmpBoard.GetResult())
                    {
                        bestResult = tmpBoard;
                    }
                }
                catch(...)
                {

                }
            }
        }
        catch(...)
        {

        }

        return bestResult.GetOrigin();
    } else
    {
        Traces() << "\n" << "ERR: Number of boards 0";

        return state.GetPrviousBoard();
    }
}

Scheduler::~Scheduler()
{
    delete condition_var;
}
