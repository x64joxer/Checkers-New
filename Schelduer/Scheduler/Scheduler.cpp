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
        MessageCoder::MessageToMap(tmpMessage.wskMessage, messageContent);

        MessageInterpreting(tmpMessage.connectionWsk, messageContent);

    }
}

void Scheduler::MessageInterpreting(TCPConnection_ptr socket, std::map<std::string, std::string> & data)
{
    Traces() << "\n" << "LOG: void Scheduler::MessageInterpreting(TCPConnection_ptr socket, std::map<std::string, std::string> & dest)";

    try
    {
        std::string action = data.at(MessageCoder::ACTION);

        if (action == MessageCoder::SET_ROLE)
        {
            Traces() << "\n" << "LOG: action == MessageCoder::SET_ROLE";

            SetRole(socket, data);
        } else
        if (action == MessageCoder::GET_SERVER_STATE)
        {
            Traces() << "\n" << "LOG: action == MessageCoder::GET_SERVER_STATE";
            SendServerState(socket, state, data);
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

void Scheduler::SetRole(TCPConnection_ptr socket, const std::map<std::string, std::string> & data)
{
    Traces() << "\n" << "LOG: void Scheduler::SetRole(TCPConnection_ptr socket, std::map<std::string, std::string> & dest)";
    try
    {
        std::string role = data.at(MessageCoder::ROLE);

        if (atoi(role.c_str()) == MessageCoder::ROLE_ENUM::CLIENT)
        {
            AddClient(socket, data);
        }
    }
    catch (std::out_of_range)
    {
        Traces() << "\n" << "ERR: Protocol error host: " << socket->GetIp() << ":" << socket->GetPort();
    }
}

void Scheduler::SendServerState(TCPConnection_ptr socket, const ServerState & serverState, const std::map<std::string, std::string> & data)
{
    Traces() << "\n" << "void Scheduler::SendState(TCPConnection_ptr socket, const ServerState & serverState, const std::map<std::string, std::string> & data)";
    try
    {
        std::string messageId = data.at(MessageCoder::MESSAGE_ID);

        char *tmpChar = new char[4048];
        MessageCoder::ClearChar(tmpChar, 4048);

        MessageCoder::CreateServerStateMessage(serverState, messageId, tmpChar);

        Traces() << "\n" << "LOG: Sending: " << tmpChar;

        socket->SendMessage(tmpChar);
        delete [] tmpChar;
    }
    catch (std::out_of_range)
    {
        Traces() << "\n" << "ERR: Protocol error host: " << socket->GetIp() << ":" << socket->GetPort();
    }
}

void Scheduler::AddClient(TCPConnection_ptr socket, const std::map<std::string, std::string> & data)
{
    Traces() << "\n" << "LOG: void Scheduler::AddClient(TCPConnection_ptr socket, const std::map<std::string, std::string> & data)";


    if (clients.Insert(socket, Client())  == true)
    {
        Traces() << "\n" << "ERR: Element already existed!";
    } else
    {
        std::string messageId = data.at(MessageCoder::MESSAGE_ID);

        char *tmpChar = new char[2048];
        MessageCoder::ClearChar(tmpChar, 2048);

        MessageCoder::CreateOkMessage(messageId, tmpChar);

        Traces() << "\n" << "LOG: Sending: " << tmpChar;

        socket->SendMessage(tmpChar);
        delete [] tmpChar;
    }
}

Scheduler::~Scheduler()
{
    delete condition_var;
}
