#include "Scheduler.h"

Scheduler::Scheduler() : wskConnectionManager(nullptr)
{
    Traces() << "\n" << "LOG: Scheduler::Scheduler()";
    condition_var = new std::condition_variable();

}

ConnectionManager * Scheduler::GetConnectionManager()
{
    std::lock_guard<std::mutex> ls(mutex);
    return wskConnectionManager;
}

void Scheduler::SetConnectionManager(ConnectionManager *wsk)
{
    std::lock_guard<std::mutex> ls(mutex);
    wskConnectionManager = wsk;
    wskConnectionManager->SetConditionVariable(condition_var);
}

void Scheduler::Start(const unsigned short numofthread)
{
    Traces() << "\n" << "LOG: void Scheduler::Start(const unsigned short numofthread)";

    for (unsigned short i=0;i<numofthread;i++)
    {
        Traces() << "\n" << "LOG: Start thread: " << i;
        iaThread[i] = std::move(std::thread(&Scheduler::StartScheduling,
                                            this));

        iaThread[i].detach();
    }
}

void Scheduler::StartScheduling()
{
    Traces() << "\n" << "LOG: void Scheduler::StartScheduling()";

    while (true)
    {

    }
}

Scheduler::~Scheduler()
{
    delete condition_var;
}
