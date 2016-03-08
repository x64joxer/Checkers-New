#include "Scheduler.h"

Scheduler::Scheduler()
{
    Traces() << "\n" << "LOG: Scheduler::Scheduler()";
    messageQueue = new SharedPtrList<Message>;
}

void Scheduler::NewConnection(TCPConnection_ptr wsk)
{
    Traces() << "\n" << "LOG: Adding new connection";    

    wsk->SetMessageQueue(messageQueue);
    connections_.insert(wsk);
    wsk->Start();
}

void Scheduler::Init()
{

}

Scheduler::~Scheduler()
{
    Traces() << "\n" << "LOG: Scheduler::~Scheduler()";
    delete messageQueue;
}
