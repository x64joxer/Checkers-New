#include "QueueTimer.h"

QueueTimer::QueueTimer() : messageQueue(nullptr),
                           stop_flag(nullptr)
{

}

void QueueTimer::doSleep(const unsigned int sleep_time, std::atomic_bool *stop_f)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));

    if (*stop_f == false)
    {        
        messageQueue->PushBack(messageToSend);
    }

    delete stop_f;
}

void QueueTimer::SetMessageToSend(const Message &message)
{
    messageToSend = message;
}

void QueueTimer::SetTime(const unsigned int miliseconds)
{
    time = miliseconds;
}

void QueueTimer::SetQueue(SharedPtrList<Message> *queue)
{
    messageQueue = queue;
}


void QueueTimer::Start()
{
    if (stop_flag) Stop();
    stop_flag = new std::atomic_bool;
    *stop_flag = false;
    thread = std::move(std::thread(&QueueTimer::doSleep, this, time, stop_flag));
    thread.detach();
}

void QueueTimer::Stop()
{
    *stop_flag = true;
}
