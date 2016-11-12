#ifndef QUEUETIMER_H
#define QUEUETIMER_H


#include <atomic>
#include <condition_variable>
#include <thread>
#include <mutex>
#include <chrono>
#include "../SafeQueue/SharedPtrSet/SharedPtrList.h"
#include "../Scheduler/Message.h"
#include "../Traces/Traces.h"
#include "../ProgramVariables/ProgramVariables.h"

class QueueTimer
{
    public:
        QueueTimer();
        void SetTime(const unsigned int miliseconds);
        void SetQueue(SharedPtrList<Message> *queue);
        void SetMessageToSend(const Message &message);
        void Start();
        void Stop();
        ~QueueTimer();

    private:
        void DoSleep(std::timed_mutex *mutex_guard, std::atomic_bool * stop_flag, std::atomic_bool *threadExist);

        unsigned int time;
        SharedPtrList<Message> *messageQueue;
        Message messageToSend;
        std::thread thread;        
        std::timed_mutex mutex_guard;
        std::mutex acces_mutex;
        std::atomic_bool stop_flag;
        std::atomic_bool threadExist;
        std::condition_variable mycond;

};

#endif // QUEUETIMER_H
