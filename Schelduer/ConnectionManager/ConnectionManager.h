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

        Message GetFirstMessage();
        SharedPtrList<Message> * GetMessageQueue() { return messageQueue; }
        void NewConnection(TCPConnection_ptr wsk);
        void CloseConnection(TCPConnection_ptr wsk);
        void SetConditionVariable(std::condition_variable *wsk) { messageQueue->SetCondVar(wsk); }
        bool IsNewMessage() { return !messageQueue->Empty(); }

        ~ConnectionManager();

    private:       
        TCPConnection *socket;

        // The managed connections.        
        std::set<TCPConnection_ptr> connections_;
        SharedPtrList<Message> *messageQueue;
};

#endif // ConnectionManager_H
