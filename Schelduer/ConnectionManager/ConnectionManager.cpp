#include "ConnectionManager.h"

void ConnectionManager::CloseConnection(TCPConnection_ptr wsk)
{
    TRACE_FLAG_FOR_CLASS_ConnectionManager Traces() << "\n" << "LOG: ConnectionManager::ConnectionManager()";
    connections_.erase(wsk);

    TRACE_FLAG_FOR_CLASS_ConnectionManager Traces() << "\n" << "LOG: Number of connections: " <<  connections_.size();
}

ConnectionManager::ConnectionManager()
{
    TRACE_FLAG_FOR_CLASS_ConnectionManager Traces() << "\n" << "LOG: void ConnectionManager::CloseConnection(TCPConnection_ptr wsk)";
    messageQueue = new SharedPtrList<Message>;        
}

Message ConnectionManager::GetFirstMessage()
{
    TRACE_FLAG_FOR_CLASS_ConnectionManager Traces() << "\n" << "LOG: Message ConnectionManager::GetFirstMessage()";

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
    TRACE_FLAG_FOR_CLASS_ConnectionManager Traces() << "\n" << "LOG: Adding new connection";

    wsk->SetMessageQueue(messageQueue);
    connections_.insert(wsk);
    wsk->Start();

    TRACE_FLAG_FOR_CLASS_ConnectionManager Traces() << "\n" << "LOG: Number of connections: " <<  connections_.size();
}

ConnectionManager::~ConnectionManager()
{
    TRACE_FLAG_FOR_CLASS_ConnectionManager Traces() << "\n" << "LOG: ConnectionManager::~ConnectionManager()";
    delete messageQueue;
}
