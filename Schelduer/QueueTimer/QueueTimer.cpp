#include "QueueTimer.h"

QueueTimer::QueueTimer() : messageQueue(nullptr),
                           stop_flag(false),
                           threadExist(false)
{
    TRACE_FLAG_FOR_CLASS_QueueTimer Traces() << "\n" << "LOG: QueueTimer::QueueTimer()";
}

void QueueTimer::DoSleep(std::timed_mutex *mutex_guard, std::atomic_bool * stop_flag, std::atomic_bool *threadExist)
{    
    TRACE_FLAG_FOR_CLASS_QueueTimer Traces() << "\n" << "LOG: void QueueTimer::DoSleep(std::timed_mutex *mutex_guard, std::atomic_bool * stop_flag, std::atomic_bool *threadExist)";

    auto now = std::chrono::system_clock::now();
    mutex_guard->try_lock_until(now + std::chrono::milliseconds(time));            

    if (messageQueue != nullptr)
    {
        if (*stop_flag == false) messageQueue->PushBack(messageToSend);
    }
    *threadExist = false;
    mutex_guard->unlock();    
    mycond.notify_one();
}

void QueueTimer::SetMessageToSend(const Message &message)
{
    TRACE_FLAG_FOR_CLASS_QueueTimer Traces() << "\n" << "LOG: void QueueTimer::SetMessageToSend(const Message &message)";

    std::lock_guard<std::mutex> ls(acces_mutex);
    messageToSend = message;
}

void QueueTimer::SetTime(const unsigned int miliseconds)
{
    TRACE_FLAG_FOR_CLASS_QueueTimer Traces() << "\n" << "LOG: void QueueTimer::SetTime(const unsigned int miliseconds)";

    std::lock_guard<std::mutex> ls(acces_mutex);
    time = miliseconds;
}

void QueueTimer::SetQueue(SharedPtrList<Message> *queue)
{
    TRACE_FLAG_FOR_CLASS_QueueTimer Traces() << "\n" << "LOG: void QueueTimer::SetQueue(SharedPtrList<Message> *queue)";

    std::lock_guard<std::mutex> ls(acces_mutex);
    messageQueue = queue;
}


void QueueTimer::Start()
{    
    TRACE_FLAG_FOR_CLASS_QueueTimer Traces() << "\n" << "LOG: void QueueTimer::Start()";

    Stop();

    threadExist = true;
    mutex_guard.lock();
    thread = std::move(std::thread(&QueueTimer::DoSleep, this, &mutex_guard, &stop_flag, &threadExist));
    thread.detach();
}

void QueueTimer::Stop()
{
    TRACE_FLAG_FOR_CLASS_QueueTimer Traces() << "\n" << "LOG: void QueueTimer::Stop()";

    if (threadExist)
    {
        stop_flag = true;
        mutex_guard.unlock();       

        std::unique_lock<std::mutex> guard(acces_mutex);
        mycond.wait(guard,[this]{ return (float)!threadExist; });

        stop_flag = false;
    };
}

QueueTimer::~QueueTimer()
{
    Stop();
}
