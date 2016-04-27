#include "Worker.h"

Worker::Worker()
{
    Traces() << "\n" << "LOG: Worker::Worker()";

    messageQueue = new SharedPtrList<Message>;
}

void Worker::Start()
{
    Traces() << "\n" << "LOG: void Worker::Start()";

    socketToServer.SetMessageQueue(messageQueue);
    socketToServer.Connect("192.168.0.7", "6000");

    workerThread = std::move(std::thread(&Worker::StartWorking,
                                          this));

    workerThread.join();
}

void Worker::StartWorking()
{
    Traces() << "\n" << "LOG: void Worker::StartWorking()";

    condition_var = messageQueue->GetCondVar();
    Message tmpMessage;

    while(true)
    {
        Traces() << "\n" << "LOG: Waiting for a job..";
        std::mutex tmpMutex;
        std::unique_lock<std::mutex> guard(tmpMutex);

        condition_var->wait(guard,[this]
        {
            return !messageQueue->Empty();
        }
        );

        tmpMessage = messageQueue->PopFront();

    }
}

Worker::~Worker()
{
    Traces() << "\n" << "LOG: Worker::~Worker()";
    delete messageQueue;
}
