#include "QueueTimer.h"

QueueTimer::QueueTimer() : messageQueue(nullptr),
                           stop_flag(false),
                           threadExist(false)
{

}

/*void QueueTimer::DoSleep(std::atomic_bool *stop_flag, std::atomic_bool *threadExist, std::mutex *mutex_guard)
{
    bool flag = true;
    std::unique_lock<std::mutex> lock(*mutex_guard);
    mycond.wait_for( lock,
                     std::chrono::milliseconds(time),
                     [&flag]() { flag = !flag; return flag; } );

    if (*stop_flag == false) messageQueue->PushBack(messageToSend);
    *threadExist = false;
    mycond.notify_one();
}*/

void QueueTimer::DoSleep()
{
    bool flag = true;
    std::unique_lock<std::mutex> lock(mutex_guard);
    mycond.wait_for( lock,
                     std::chrono::milliseconds(time),
                     [&flag]() { flag = !flag; return flag; } );

    if (stop_flag == false) messageQueue->PushBack(messageToSend);
    threadExist = false;
    mycond.notify_one();
}

void QueueTimer::SetMessageToSend(const Message &message)
{
    std::lock_guard<std::mutex> ls(mutex_guard);
    messageToSend = message;
}

void QueueTimer::SetTime(const unsigned int miliseconds)
{
    std::lock_guard<std::mutex> ls(mutex_guard);
    time = miliseconds;
}

void QueueTimer::SetQueue(SharedPtrList<Message> *queue)
{
    std::lock_guard<std::mutex> ls(mutex_guard);
    messageQueue = queue;
}


void QueueTimer::Start()
{    
    Stop();

    threadExist = true;
    thread = std::move(std::thread(&QueueTimer::DoSleep, this));
    thread.detach();
}

void QueueTimer::Stop()
{    
    if (threadExist)
    {
        stop_flag = true;
        mycond.notify_one();

        std::unique_lock<std::mutex> lock(mutex_guard);
        mycond.wait( lock, [this]() { return !threadExist; } );

        stop_flag = false;
    };
}
