#ifndef QUEUETIMER_H
#define QUEUETIMER_H

#include <thread>
#include <atomic>
#include <mutex>
#include "../SafeQueue/SharedPtrSet/SharedPtrList.h"
#include "../Worker/Message.h"

class QueueTimer
{
    public:
        QueueTimer();
        void SetTime(const unsigned int miliseconds);
        void SetQueue(SharedPtrList<Message> *queue);
        void SetMessageToSend(const Message &message);
        void Start();
        void Stop();

    private:
        void doSleep(const unsigned int sleep_time, std::atomic_bool *stop_f);

        unsigned int time;
        SharedPtrList<Message> *messageQueue;
        Message messageToSend;
        std::thread thread;
        std::atomic_bool *stop_flag;
        std::mutex mutex_guard;

};

#endif // QUEUETIMER_H
