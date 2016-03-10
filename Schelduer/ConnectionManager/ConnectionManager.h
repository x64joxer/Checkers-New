#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include <iostream>
#include <set>
#include <thread>
#include "../TCP/TCPConnection.h"
#include "../Traces/Traces.h"
#include "../SafeQueue/SharedPtrSet/SharedPtrList.h"
#include "../SafeQueue/SharedPtrSet/SharedPtrSet.h"

class ConnectionManager
{
    public:
        ConnectionManager();
        void Init();
        void NewConnection(TCPConnection_ptr wsk);
        void SetConditionVariable(std::condition_variable *wsk) { messageQueue->SetCondVar(wsk); }
        ~ConnectionManager();

    private:       
        TCPConnection *socket;

        // The managed connections.        
        std::set<TCPConnection_ptr> connections_;
        SharedPtrList<Message> *messageQueue;
};

#endif // ConnectionManager_H
