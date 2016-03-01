#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <iostream>
#include "TCP/TCPConnection.h"

class Scheduler
{
    public:
        Scheduler();
        void Init();
        void NewConnection(TCPConnection_ptr wsk);

    private:       
        TCPConnection *socket;
};

#endif // SCHEDULER_H
