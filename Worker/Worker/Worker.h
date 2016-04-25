#ifndef WORKER_H
#define WORKER_H

#include "../TCP/TCPSocket.h"

class Worker
{
    public:
        Worker();
        void Start();

        ~Worker();

    private:
        TCPSocket socketToServer;
        SharedPtrList<Message> *messageQueue;
};

#endif // WORKER_H
