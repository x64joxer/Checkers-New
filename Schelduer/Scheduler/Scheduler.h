#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <thread>
#include <mutex>
#include "../Traces/Traces.h"
#include "../ConnectionManager/ConnectionManager.h"

class Scheduler
{
    public:
        Scheduler();

        ConnectionManager * GetConnectionManager();
        void Start(const unsigned short numofthread);
        void SetConnectionManager(ConnectionManager *wsk);
        ~Scheduler();

    private:
        void StartScheduling();

        ConnectionManager *wskConnectionManager;

        std::thread iaThread[1];
        std::mutex mutex;
        std::condition_variable *condition_var;
};

#endif // SCHEDULER_H
