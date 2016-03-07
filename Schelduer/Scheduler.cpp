#include "Scheduler.h"

Scheduler::Scheduler()
{
    Traces() << "\n" << "LOG: Scheduler::Scheduler()";
    messageQueue = new SharedPtrSet<boost::array<char, 8192> >;
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
