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
    bool isWorkerToStopAnalyse;

    while (true)
    {
        TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: Waiting for a job..";
        std::mutex tmpMutex;
        std::unique_lock<std::mutex> guard(tmpMutex);        

        condition_var->wait(guard,[this, &isNewBoardToAnalyse, &isNewMessage, &isClientToUpate, &isWorkerToStopAnalyse]
        {
            /////////////////////////
            //Traffic test purposes
            /////////////////////////
            if (ProgramVariables::GetTrafficFlag())
            {
                if (boardsToAnalyse.Size() < 2 * workers.Size())
                {
                    unsigned int tmpSize = freeWorkers.Size();

                    Board tmpBoard;
                    tmpBoard =
                            std::string("| |w| |w| |w| |w|") +
                            std::string("|w| |w| |w| |w| |") +
                            std::string("| |w| |w| |w| |w|") +
                            std::string("| | | | | | | | |") +
                            std::string("| | | | | | | | |") +
                            std::string("|b| |b| |b| |b| |") +
                            std::string("| |b| |b| |b| |b|") +
                            std::string("|b| |b| |b| |b| |");

                    for(int i=0; i < (tmpSize*2);i++)
                    {
                        boardsToAnalyse.PushBack(tmpBoard);
                    }
                }
            }
            /////////////////////////
            //End traffic test purposes
            ////////////////////////

            //Check if new board to analyse are present
            isNewBoardToAnalyse = CheckIfNewBoardToAnalyse();

            //Check if new messagee to analyse are present
            isNewMessage = wskConnectionManager->IsNewMessage();

            //Check if clinet to update are present
            isClientToUpate = !clientsToStateUpdate.Empty();

            //Check if worker to stop analyse are present
            isWorkerToStopAnalyse = !workersToStopAnalyse.Empty();

            return (isNewBoardToAnalyse | isNewMessage | isClientToUpate | isWorkerToStopAnalyse); }
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
        } else
        if (isClientToUpate)
        {
            TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: Client to update";

            UpdateNextClientStatus(tmpTCP_Connection_ptr, dest);
        } else
        if (isWorkerToStopAnalyse)
        {
            TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: Workers to send stop analyse";

            SendStopAnalyseToWorker(tmpTCP_Connection_ptr, dest);
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

            CloseConnection(socket);
        } else
        if (action == MessageCoder::RESET_SERVER_STATE)
        {
            TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: action == MessageCoder::RESET_SERVER_STATE";

            ResetServerState(socket, data, dest);
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

            StartWork(socket, data, dest);
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

            ReceiveOKMessage(socket, data, dest);
        } else
        if (action == MessageCoder::NOT_OK)
        {
            TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: action == MessageCoder::NOT_OK";

            ReceiveNotOKMessage(socket, data, dest);
        } else
        if (action == MessageCoder::TIMEOUT)
        {
            TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: action == MessageCoder::TIMEOUT";

            ReceiveTimeoutMessage(socket, data, dest);
        }
        else
        {
            Traces() << "\n" << "ERR: Unexpected action from host";
        }

    }
    catch (std::out_of_range)
    {
        Traces() << "\n" << "ERR: Protocol error host";
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
        Traces() << "\n" << "ERR: Protocol error host";
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
        Traces() << "\n" << "ERR: Protocol error host";
    }
}

void Scheduler::SetState(TCPConnection_ptr socket, Peers::STATE state)
{
    TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: void Scheduler::SetState(TCPConnection_ptr socket, Peers::STATE state)";
    try
    {
       Worker_ptr tmpWorker = workers.At(socket);
       tmpWorker->SetState(state);
       UpdateFreeWorkerList(socket, tmpWorker);
    }
    catch (...)
    {
        Traces() << "\n" << "ERR: No such worker";
    }
}

void Scheduler::SendStopAnalyse(TCPConnection_ptr socket, const std::string & messageId, char * dest)
{
    TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: void Scheduler::SendStopAnalyse(TCPConnection_ptr socket, const std::string & messageId, char * dest)";

    try
    {
        MessageCoder::ClearChar(dest, MessageCoder::MaxMessageSize());
        MessageCoder::CreateStopAnalyse(messageId, dest);

        TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: Sending: " << dest;

        socket->SendMessage(dest);
    }
    catch (std::out_of_range)
    {
        Traces() << "\n" << "ERR: Protocol error host";
    }
}

void Scheduler::SendServerState(TCPConnection_ptr socket, const ServerState & serverState, const std::string & messageId, char * dest)
{
    TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: TCPConnection_ptr socket, const ServerState & serverState, const std::string & messageId, char * dest";

    try
    {
        MessageCoder::ClearChar(dest, MessageCoder::MaxMessageSize());
        MessageCoder::CreateServerStateMessage(serverState, messageId, dest);

        TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: Sending: " << dest;

        socket->SendMessage(dest);
    }
    catch (std::out_of_range)
    {
        Traces() << "\n" << "ERR: Protocol error host";
    }
}

void Scheduler::SendServerState(TCPConnection_ptr socket, const ServerState & serverState, const std::map<std::string, std::string> & data, char * dest)
{
    TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: void Scheduler::SendState(TCPConnection_ptr socket, const ServerState & serverState, const std::map<std::string, std::string> & data, char * dest)";

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
        Traces() << "\n" << "ERR: Protocol error host";
    }
}

void Scheduler::StartWork(TCPConnection_ptr socket, const std::map<std::string, std::string> & data, char * dest)
{
    TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: void Scheduler::StartWork(TCPConnection_ptr socket, const std::map<std::string, std::string> & data, char * dest)";

    if (freeWorkers.Size() == 0)
    {
        state.SetlastServerError("No free workers!");
    } else
    {
        Counters::ClearCounterNumberOfAnalysedBoard();

        workOngoing = true;

        Board tmpBoard;
        tmpBoard = state.GetBoard();
        state.SetPreviousBoard(tmpBoard);

        MessageCoder::MapToBoard(data, &tmpBoard);
        state.SetBoard(tmpBoard);
        state.SetThinking(true);
        unsigned long long tmpTime = std::atoi(data.at(MessageCoder::MAX_TIME).c_str());
        state.SetMaxTime(tmpTime);
        state.SetMaxTimeForWorkers(tmpTime - ProgramVariables::GetTimeReserveToSendBestResultToClient());
        state.SetStartTime(Traces::GetMilisecondsSinceEpoch());
        CreateTimeToSendResultToClientsGuard(socket, state.GetMaxTimeForWorkers());

        boardsToAnalyse.Clear();
        /////////////////////////
        //Traffic test purposes
        /////////////////////////
        if (ProgramVariables::GetTrafficFlag())
        {
            unsigned int tmpSize = freeWorkers.Size();

            for(int i=0; i < (tmpSize*5);i++)
            {
                boardsToAnalyse.PushBack(tmpBoard);
            }
        } else
        /////////////////////////
        //End traffic test purposes
        ////////////////////////
        {
            tmpBoard.SetOrigin(tmpBoard);
            boardsToAnalyse.PushBack(tmpBoard);
        }
    }

    SendStateToAllClients(data, dest);
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

void Scheduler::CloseConnection(TCPConnection_ptr socket)
{
    TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: void Scheduler::CloseConnection(TCPConnection_ptr socket)";

    bool removeResult;

    removeResult = RemoveClient(socket);
    if (!removeResult) removeResult = RemoveWorker(socket);
}

bool Scheduler::CheckIfNewBoardToAnalyse()
{
    TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: bool Scheduler::CheckIfNewBoardToAnalyse()";

    bool isNewBoardToAnalyse;

    isNewBoardToAnalyse = (!boardsToAnalyse.Empty()) & (!freeWorkers.Empty()) & (workOngoing);
    if (Traces::GetMilisecondsSinceEpoch() > ((state.GetStartTime() + state.GetMaxTimeForWorkers()) - (ProgramVariables::GetTimeReserveToSendBestResultToClient() + ProgramVariables::GetTimeToSendJobsToFreeWorkers()))) isNewBoardToAnalyse = false;

    return isNewBoardToAnalyse;
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
        freeWorkers.Remove(socket);
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

void Scheduler::ResetServerState(TCPConnection_ptr socket, const std::map<std::string, std::string> & data, char * dest)
{
    TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: void Scheduler::ResetServerState(TCPConnection_ptr socket, const std::map<std::string, std::string> & data, char * dest)";

    Board tmpBoard;
    MessageCoder::MapToBoard(data, &tmpBoard);
    state.SetBoard(tmpBoard);
    state.SetThinking(false);
    boardsToAnalyse.Clear();
    jobTimer.Stop();

    SendStateToAllClients(data, dest);
    workers.GetAllKeys(workersToStopAnalyse);
}

void Scheduler::ReceiveOKMessage(TCPConnection_ptr socket, const std::map<std::string, std::string> & data, char * dest)
{
    TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: void Scheduler::ReceiveOKMessage(TCPConnection_ptr socket, const std::map<std::string, std::string> & data, char * dest)";

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

            if (tmpWorker->GetConnectionState() == Worker::ConnectionState::WaitForOkMessageAfterSendFirstJob)
            {
                timerList.RemoveFromList(socket);
                tmpWorker->SetConnectionState(Worker::ConnectionState::None);
                tmpWorker->SetState(Peers::BUSY);
            } else
            if (tmpWorker->GetConnectionState() == Worker::ConnectionState::WaitForOkMessageAfterSendJob)
            {
                timerList.RemoveFromList(socket);
                tmpWorker->SetConnectionState(Worker::ConnectionState::None);
                tmpWorker->SetState(Peers::BUSY);
            } else
            if (tmpWorker->GetConnectionState() == Worker::ConnectionState::WaitForOkMessageAfterSendStopAnnalyse)
            {
                timerList.RemoveFromList(socket);
                tmpWorker->SetConnectionState(Worker::ConnectionState::None);
                SetState(socket, Peers::FREE);
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

}

void Scheduler::ReceiveNotOKMessage(TCPConnection_ptr socket, const std::map<std::string, std::string> & data, char * dest)
{
    TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: void Scheduler::ReceiveNotOKMessage(TCPConnection_ptr socket, const std::map<std::string, std::string> & data, char * dest)";

    bool okFlag = false;

    try
    {
        Worker_ptr tmpWorker = workers.At(socket);

        TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: Worker found on the tiemr list";

        if (tmpWorker->GetConnectionState() == Worker::ConnectionState::WaitForOkMessageAfterSendFirstJob)
        {
            MessageCoder::NOT_OK_REASON tmpReason = static_cast<MessageCoder::NOT_OK_REASON>(std::atoi(data.at(MessageCoder::REASON).c_str()));

            if (tmpReason == MessageCoder::NOT_OK_REASON::BUSY)
            {
                timerList.RemoveFromList(socket);
                tmpWorker->SetState(Peers::BUSY);
                firstJobStarted = false;
                boardsToAnalyse.PushBack(state.GetBoard());
            } else
            {
                Traces() << "\n" << "ERR: Unexpected NOT_OK reason from worker";
            }

        } else
        if (tmpWorker->GetConnectionState() == Worker::ConnectionState::WaitForOkMessageAfterSendJob)
        {
            MessageCoder::NOT_OK_REASON tmpReason = static_cast<MessageCoder::NOT_OK_REASON>(std::atoi(data.at(MessageCoder::REASON).c_str()));

            if (tmpReason == MessageCoder::NOT_OK_REASON::BUSY)
            {
                timerList.RemoveFromList(socket);
                tmpWorker->SetState(Peers::BUSY);
            } else
            {
                Traces() << "\n" << "ERR: Unexpected NOT_OK reason from worker";
            }
        } else
        {
            Traces() << "\n" << "ERR: Unexpected NOT_OK message from worker";
        }

        okFlag = true;
    }
    catch (const std::out_of_range& oor)
    {

    }
}

void Scheduler::ReceiveTimeoutMessage(TCPConnection_ptr socket, const std::map<std::string, std::string> & data, char * dest)
{
    TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: void Scheduler::ReceiveTimeout(TCPConnection_ptr socket, const std::map<std::string, std::string> & data, char * dest)";

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

        if (tmpWorker->GetConnectionState() == Worker::ConnectionState::WaitForOkMessageAfterSendFirstJob)
        {
            socket->Close();
            firstJobStarted = false;
            boardsToAnalyse.PushBack(state.GetBoard());
        } else
        if (tmpWorker->GetConnectionState() == Worker::ConnectionState::WaitForOkMessageAfterSendJob)
        {
            socket->Close();
        } else
        if (tmpWorker->GetConnectionState() == Worker::ConnectionState::WaitForOkMessageAfterSendStopAnnalyse)
        {
            socket->Close();
        }
    }
    catch (const std::out_of_range& oor)
    {

    }
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

    jobTimer.SetTime(miliseconds);
    jobTimer.SetQueue(wskConnectionManager->GetMessageQueue());
    char *tmpData = new char[MessageCoder::MaxMessageTimeToSendResultToClientsSize()];
    MessageCoder::ClearChar(tmpData, MessageCoder::MaxMessageTimeToSendResultToClientsSize());
    MessageCoder::CreateTimeToSendResultToClientsMessage(tmpData);
    Message tmpMessage;
    tmpMessage.CopyWsk(nullptr, tmpData);
    jobTimer.SetMessageToSend(tmpMessage);
    jobTimer.Start();
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
    bool exceptionOccured = false;
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
            exceptionOccured = true;
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
                exceptionOccured = true;
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
                exceptionOccured = true;
                listEmpty = true;
            }
        } else
        {
            if (!exceptionOccured) freeWorkers.PushBack(tmpWorkerSocket);
        }

        //Check if it is first job
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
        } else
        {
            if (!exceptionOccured) freeWorkers.PushBack(tmpWorkerSocket);
        }

        if (!exceptionOccured)
        {
            if (!tmpFirstJobStarted) firstJobStarted = true;
        }
    }

    //Send job
    if (!exceptionOccured)
    {
        if (!listEmpty)
        {
            if ((!tmpFirstJobStarted) && (!tmpSizeMoreThanOne))
            {
                TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: Send job to only one worker";

                std::string messageId = MessageCoder::CreateMessageId();
                std::string jobId = MessageCoder::CreateMessageId();
                MessageCoder::ClearChar(dest, MessageCoder::MaxMessageSize());



                MessageCoder::CreateStartAnalyseWork(state.GetMaxTimeForWorkers() - (Traces::GetMilisecondsSinceEpoch() - state.GetStartTime()),
                                                     tmpBoard,
                                                     messageId,
                                                     jobId,
                                                     true,
                                                     dest);

                tmpWorker->SetConnectionState(Worker::ConnectionState::WaitForOkMessageAfterSendJob);
                tmpWorkerSocket->SendMessage(dest);
                CreateTimeoutGuard(tmpWorkerSocket, ProgramVariables::GetMaxTimeoutForMessageResponse());
            } else
            if ((!tmpFirstJobStarted) && (tmpSizeMoreThanOne))
            {
                TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: Send job and order worker to return N-result fast";

                std::string messageId = MessageCoder::CreateMessageId();
                std::string jobId = MessageCoder::CreateMessageId();
                MessageCoder::ClearChar(dest, MessageCoder::MaxMessageSize());



                MessageCoder::CreateStartAnalyseWorkAndReturnNResultFast(state.GetMaxTimeForWorkers() - (Traces::GetMilisecondsSinceEpoch() - state.GetStartTime()),
                                                                         tmpFreeWorkerListSize - 1,
                                                                         tmpBoard,
                                                                         messageId,
                                                                         jobId,
                                                                         dest);

                tmpWorker->SetConnectionState(Worker::ConnectionState::WaitForOkMessageAfterSendFirstJob);
                tmpWorkerSocket->SendMessage(dest);
                CreateTimeoutGuard(tmpWorkerSocket, ProgramVariables::GetMaxTimeoutForMessageResponse());

            } else
            if (tmpFirstJobStarted)
            {
                TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: Send job to worker";

                std::string messageId = MessageCoder::CreateMessageId();
                std::string jobId = MessageCoder::CreateMessageId();
                MessageCoder::ClearChar(dest, MessageCoder::MaxMessageSize());



                MessageCoder::CreateStartAnalyseWork(state.GetMaxTimeForWorkers() - (Traces::GetMilisecondsSinceEpoch() - state.GetStartTime()),
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

void Scheduler::SendStopAnalyseToWorker(TCPConnection_ptr tmpTCP_Connection_ptr, char * dest)
{
    TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: void Scheduler::SendStopanalyseToWorker(TCPConnection_ptr tmpTCP_Connection_ptr, char * dest)";

    bool wasWorkerToUpdate = true;

    try
    {
        tmpTCP_Connection_ptr = workersToStopAnalyse.PopFront();
    }
    catch (std::string)
    {
        wasWorkerToUpdate = false;
        TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: List empty. Not a bug.";
    }

    if (wasWorkerToUpdate)
    {
        if (workers.At(tmpTCP_Connection_ptr)->GetState() == Peers::STATE::BUSY)
        {
           SendStopAnalyse(tmpTCP_Connection_ptr, MessageCoder::CreateMessageId(), dest);
           workers.At(tmpTCP_Connection_ptr)->SetConnectionState(Worker::ConnectionState::WaitForOkMessageAfterSendStopAnnalyse);
           CreateTimeoutGuard(tmpTCP_Connection_ptr, ProgramVariables::GetMaxTimeoutForMessageResponse());
        }
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

    Counters::AddToCounterNumberOfAnalysedBoard(atoi(data.at(MessageCoder::NUM_OF_ANALYSED).c_str()));

    TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: Sending: " << dest;
    socket->SendMessage(dest);
}

void Scheduler::FinishWork(const std::map<std::string, std::string> & data, char * dest)
{
    TRACE_FLAG_FOR_CLASS_Scheduler Traces() << "\n" << "LOG: void Scheduler::FinishWork(const std::map<std::string, std::string> & data, char * dest)";

    jobTimer.Stop();
    workOngoing = false;
    firstJobStarted = false;    
    state.SetThinking(false);

    if (boardsToAnalyse.Size() > 0)
    {
        state.SetBoard(CalculateBestResult());

        state.SetlastServerError(ServerState::NO_SERVER_ERROR_TEXT);
    } else
    {
        Traces() << "\n" << "ERR: No result to send";

        state.SetlastServerError("No result received form workers!");
    }

    SendStateToAllClients(data, dest);

    TRACE_FLAG_FOR_Notif Traces() << "\n" << "LOG:****************************************************************";
    TRACE_FLAG_FOR_Notif Traces() << "\n" << "LOG:****************TOTAL NUMBER OF ANALYSED BOARDS*****************";

    std::string number;
    std::stringstream strstream;
    strstream << Counters::GetCounterNumberOfAnalysedBoard();
    strstream >> number;

    unsigned short firstColum = 33 - (strlen(number.c_str())/2);
    std::string whiteSigns = "";
    for(unsigned short i=0;i<firstColum;i++) whiteSigns+=" ";

    TRACE_FLAG_FOR_Notif Traces() << "\n" << "LOG:" << whiteSigns << number;
    TRACE_FLAG_FOR_Notif Traces() << "\n" << "LOG:****************************************************************";
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
