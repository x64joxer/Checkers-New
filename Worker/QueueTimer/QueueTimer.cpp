#include "QueueTimer.h"

QueueTimer::QueueTimer() : messageQueue(nullptr),
                           stop_flag(false),
                           threadExist(false)
{

}

void QueueTimer::DoSleep(std::timed_mutex *mutex_guard, std::atomic_bool * stop_flag, std::atomic_bool *threadExist)
{    
    auto now = std::chrono::system_clock::now();
    mutex_guard->try_lock_until(now + std::chrono::milliseconds(time));

    if (messageQueue != nullptr)
    {
        if (*stop_flag == false) messageQueue->PushBack(messageToSend);
    }
    *threadExist = false;
    mutex_guard->unlock();
}

void QueueTimer::SetMessageToSend(const Message &message)
{
    std::lock_guard<std::mutex> ls(acces_mutex);
    messageToSend = message;
}

void QueueTimer::SetTime(const unsigned int miliseconds)
{
    std::lock_guard<std::mutex> ls(acces_mutex);
    time = miliseconds;
}

void QueueTimer::SetQueue(SharedPtrList<Message> *queue)
{
    std::lock_guard<std::mutex> ls(acces_mutex);
    messageQueue = queue;
}


void QueueTimer::Start()
{    
    Stop();

    threadExist = true;
    mutex_guard.lock();
    thread = std::move(std::thread(&QueueTimer::DoSleep, this, &mutex_guard, &stop_flag, &threadExist));
    thread.detach();
}

void QueueTimer::Stop()
{
    if (threadExist)
    {
        stop_flag = true;
        mutex_guard.unlock();
        while (threadExist == 1) {}
        stop_flag = false;
    };
}
