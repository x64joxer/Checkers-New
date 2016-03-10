#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <thread>
#include "../Traces/Traces.h"

class Scheduler
{
    public:
        Scheduler();
        void Start(const unsigned short numofthread);

    private:
        void StartScheduling();

        std::thread iaThread[8];
};

#endif // SCHEDULER_H
