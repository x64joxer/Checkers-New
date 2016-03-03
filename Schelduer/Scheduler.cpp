#include "Scheduler.h"

Scheduler::Scheduler()
{
    Traces() << "\n" << "LOG: Scheduler::Scheduler()";
}

void Scheduler::NewConnection(TCPConnection_ptr wsk)
{
    Traces() << "\n" << "LOG: Adding new connection";

    connections_.insert(wsk);
    wsk->Start();
}

void Scheduler::Init()
{

}
