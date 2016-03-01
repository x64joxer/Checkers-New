#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <iostream>
#include "../Common/TCP/TCPConnection.h"
#include "../Common/TCP/TCPServer.h"

class Scheduler
{
    public:
        Scheduler();
        void Init();

    private:
        TCPConnection *socket;
        TCPServer *server;
};

#endif // SCHEDULER_H
