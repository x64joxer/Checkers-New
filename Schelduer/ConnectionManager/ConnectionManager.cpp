#include "ConnectionManager.h"

void ConnectionManager::CloseConnection(TCPConnection_ptr wsk)
{
    Traces() << "\n" << "LOG: ConnectionManager::ConnectionManager()";
    connections_.erase(wsk);

    Traces() << "\n" << "LOG: Number of connections: " <<  connections_.size();
}

ConnectionManager::ConnectionManager()
{
    Traces() << "\n" << "LOG: void ConnectionManager::CloseConnection(TCPConnection_ptr wsk)";
    messageQueue = new SharedPtrList<Message>;        
}

Message ConnectionManager::GetFirstMessage()
{
    Traces() << "\n" << "LOG: Message ConnectionManager::GetFirstMessage()";

    try
    {
       return messageQueue->PopFront();
    }
    catch (...)
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

    Traces() << "\n" << "LOG: Number of connections: " <<  connections_.size();
}

ConnectionManager::~ConnectionManager()
{
    Traces() << "\n" << "LOG: ConnectionManager::~ConnectionManager()";
    delete messageQueue;
}
