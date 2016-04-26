#ifndef WORKER_H
#define WORKER_H

#include <thread>
#include <mutex>
#include "../TCP/TCPSocket.h"

class Worker
{
    public:
        Worker();
        void Start();

        ~Worker();

    private:
        void StartWorking();

        std::thread workerThread;
        TCPSocket socketToServer;
        SharedPtrList<Message> *messageQueue;
        std::condition_variable *condition_var;
};

#endif // WORKER_H
