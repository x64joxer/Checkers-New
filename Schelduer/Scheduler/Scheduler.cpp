#include "Scheduler.h"

Scheduler::Scheduler() : wskConnectionManager(nullptr)
{
    Traces() << "\n" << "LOG: Scheduler::Scheduler()";
    condition_var = new std::condition_variable();

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

    while (true)
    {
        Traces() << "\n" << "LOG: Waiting for a job..";
        std::mutex tmpMutex;
        std::unique_lock<std::mutex> guard(tmpMutex);

        condition_var->wait(guard,[this]
        {
            return wskConnectionManager->IsNewMessage(); }
        );

        Traces() << "\n" << "LOG: Try reading message from message queue";

        try
        {
            tmpMessage = wskConnectionManager->GetFirstMessage();            
        }
        catch (std::string)
        {
            Traces() << "\n" << "LOG: List empty. Not a bug.";
        }

        std::map<std::string, std::string> messageContent;
        MessageCoder::MessageToMap(tmpMessage.GetWskMessage(), messageContent);

        MessageInterpreting(tmpMessage.GetTCPConnection_ptr(), messageContent, dest);
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
        }
        if (action == MessageCoder::START_WORK)
        {
            Traces() << "\n" << "LOG: action == MessageCoder::START_WORK";

            Board tmpBoard;
            MessageCoder::MapToBoard(data, &tmpBoard);
            state.SetBoard(tmpBoard);
            state.SetThinking(true);
            SendServerState(socket, state, data, dest);
            clients.At(socket).SetConnectionState(Client::ConnectionState::WaitForOkMessageAfterSendStatus);
            CreateTimeoutGuard(socket, 5000);            
        } else
        if (action == MessageCoder::OK)
        {
            Traces() << "\n" << "LOG: action == MessageCoder::OK";

            try
            {
                Client & tmpClinet = clients.At(socket);

                Traces() << "\n" << "LOG: Client found on the tiemr list";

                if (tmpClinet.GetConnectionState() == Client::ConnectionState::WaitForOkMessageAfterSendStatus)
                {
                    timerList.RemoveFromList(socket);
                    tmpClinet.SetConnectionState(Client::ConnectionState::None);
                } else
                {
                    Traces() << "\n" << "ERR: Unexpected OK message from client";
                }

            }
            catch (const std::out_of_range& oor)
            {

            }

        } else
        if (action == MessageCoder::TIMEOUT)
        {
            Traces() << "\n" << "LOG: action == MessageCoder::TIMEOUT";

            try
            {
                clients.At(socket);
                Traces() << "\n" << "LOG: Client found on the tiemr list";
                socket->Stop();
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
        {
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
       Worker & tmpWorker = workers.At(socket);
       tmpWorker.SetMaxThread(std::stoi(data.at(MessageCoder::NUM_OF_THREAD)));
       tmpWorker.SetState(static_cast<Peers::STATE>(std::stoi(data.at(MessageCoder::STATE))));

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


    if (clients.Insert(socket, Client())  == true)
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

    if (workers.Insert(socket, Worker())  == true)
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
        workers.At(socket);
        workers.Erase(socket);
        wskConnectionManager->CloseConnection(socket);
    }
    catch (const std::out_of_range& oor)
    {
        flag = true;
    }

    if (!flag) Traces() << "\n" << "LOG: Worker removed";

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

Scheduler::~Scheduler()
{
    delete condition_var;
}
