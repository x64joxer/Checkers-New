#ifndef WORKER_H
#define WORKER_H

#include <thread>
#include <mutex>
#include "../TCP/TCPSocket.h"
#include "MessageCoder.h"
#include "../QueueTimer/QueueTimer.h"
#include "../Peers/Peers.h"

class Worker
{
    public:
        Worker();
        void Start();

        ~Worker();

    private:
        void StartWorking();

        void SendRegisterMessage(TCPSocket_ptr socket, char * dest, std::string & prevousMessageid);
        void SendStateMessage(TCPSocket_ptr socket, char * dest, std::string & prevousMessageid);

        void MessageInterpreting(TCPSocket_ptr socket, std::map<std::string, std::string> & data, char * dest, QueueTimer & reconnectionTimer, std::string & prevousMessageid);

        std::thread workerThread;
        unsigned int maxThread;
        TCPSocket socketToServer;
        SharedPtrList<Message> *messageQueue;
        std::condition_variable *condition_var;
        enum ConState { DISCONNECTED, CONNECTED, REGISTERED, STATEUPDATED } connection_state;
        Peers::STATE myState;
};

#endif // WORKER_H
