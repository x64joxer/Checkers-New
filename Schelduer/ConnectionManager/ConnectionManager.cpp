#include "ConnectionManager.h"

ConnectionManager::ConnectionManager()
{
    Traces() << "\n" << "LOG: ConnectionManager::ConnectionManager()";
    messageQueue = new SharedPtrList<Message>;
}

void ConnectionManager::NewConnection(TCPConnection_ptr wsk)
{
    Traces() << "\n" << "LOG: Adding new connection";    

    wsk->SetMessageQueue(messageQueue);
    connections_.insert(wsk);
    wsk->Start();
}

void ConnectionManager::Init()
{

}

ConnectionManager::~ConnectionManager()
{
    Traces() << "\n" << "LOG: ConnectionManager::~ConnectionManager()";
    delete messageQueue;
}
