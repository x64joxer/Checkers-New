#include "Scheduler.h"

Scheduler::Scheduler()
{
    Traces() << "\n" << "LOG: Scheduler::Scheduler()";
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
