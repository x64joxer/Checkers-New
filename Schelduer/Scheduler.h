#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <iostream>
#include <set>
#include "TCP/TCPConnection.h"
#include "Traces/Traces.h"
#include "SafeQueue/SharedPtrSet/SharedPtrList.h"
#include "SafeQueue/SharedPtrSet/SharedPtrSet.h"

class Scheduler
{
    public:
        Scheduler();
        void Init();
        void NewConnection(TCPConnection_ptr wsk);
        ~Scheduler();

    private:       
        TCPConnection *socket;

        // The managed connections.        
        std::set<TCPConnection_ptr> connections_;
        SharedPtrList<Message> *messageQueue;
};

#endif // SCHEDULER_H
