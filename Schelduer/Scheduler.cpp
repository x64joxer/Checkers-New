#include "Scheduler.h"

Scheduler::Scheduler()
{

}

void Scheduler::NewConnection(TCPConnection_ptr wsk)
{
    std::cout << "New connection" << std::endl;
    connections_.insert(wsk);
    wsk->Start();
}

void Scheduler::Init()
{

}
