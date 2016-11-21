#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include <iostream>
#include <set>
#include <thread>
#include "../TCP/TCPConnection.h"
#include "../Traces/Traces.h"
#include "../SafeQueue/SharedPtrSet/SharedPtrList.h"
#include "../SafeQueue/SharedPtrSet/SharedPtrSet.h"
#include "../ProgramVariables/ProgramVariables.h"

class ConnectionManager
{
    public:
        ConnectionManager();

        Message GetFirstMessage();
        SharedPtrList<Message> * GetMessageQueue() const { return messageQueue; }
        void NewConnection(TCPConnection_ptr wsk);
        void CloseConnection(TCPConnection_ptr wsk);
        void SetConditionVariable(std::condition_variable *wsk) { messageQueue->SetCondVar(wsk); }
        bool IsNewMessage() const { return !messageQueue->Empty(); }

        ~ConnectionManager();

    private:       
        TCPConnection *socket;

        // The managed connections.        
        std::set<TCPConnection_ptr> connections_;
        SharedPtrList<Message> *messageQueue;
};

#endif // ConnectionManager_H
