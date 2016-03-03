#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <iostream>
#include <set>
#include "TCP/TCPConnection.h"
#include "Traces/Traces.h"

class Scheduler
{
    public:
        Scheduler();
        void Init();
        void NewConnection(TCPConnection_ptr wsk);

    private:       
        TCPConnection *socket;

        // The managed connections.
        std::set<TCPConnection_ptr> connections_;
};

#endif // SCHEDULER_H
