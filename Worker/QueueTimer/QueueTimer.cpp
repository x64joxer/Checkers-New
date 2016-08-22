#include "QueueTimer.h"

QueueTimer::QueueTimer() : messageQueue(nullptr),
                           stop_flag(nullptr)
{
    stop_flag = new std::atomic_bool;
}

void QueueTimer::doSleep(const unsigned int sleep_time, std::atomic_bool *stop_f)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));

    std::lock_guard<std::mutex> ls(mutex_guard);

    if (*stop_f == false)
    {        
        messageQueue->PushBack(messageToSend);
        *stop_f = true;
    } else
    {
        delete stop_f;
    }

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
    std::lock_guard<std::mutex> ls(mutex_guard);
    if (!*stop_flag)
    {
        Stop();
    } else
    {
        delete stop_flag;
    }

    stop_flag = new std::atomic_bool;
    *stop_flag = false;
    thread = std::move(std::thread(&QueueTimer::doSleep, this, time, stop_flag));
    thread.detach();
}

void QueueTimer::Stop()
{    
    *stop_flag = true;
}
