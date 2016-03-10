#include "ConnectionManager.h"

ConnectionManager::ConnectionManager()
{
    Traces() << "\n" << "LOG: ConnectionManager::ConnectionManager()";
    messageQueue = new SharedPtrList<Message>;
}

Message ConnectionManager::GetFirstMessage()
{
    Traces() << "\n" << "LOG: Message ConnectionManager::GetFirstMessage()";

    try
    {
       return messageQueue->PopFront();
    }
    catch (std::string)
    {
        throw;
    }
}

void ConnectionManager::NewConnection(TCPConnection_ptr wsk)
{
    Traces() << "\n" << "LOG: Adding new connection";    

    wsk->SetMessageQueue(messageQueue);
    connections_.insert(wsk);
    wsk->Start();
}

ConnectionManager::~ConnectionManager()
{
    Traces() << "\n" << "LOG: ConnectionManager::~ConnectionManager()";
    delete messageQueue;
}
