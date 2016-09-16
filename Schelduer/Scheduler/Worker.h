#ifndef WORKER_H
#define WORKER_H

#include <map>
#include "../Peers/Peers.h"
#include "MessageCoder.h"

class Worker
{
    public:
        Worker();
        Worker(const std::map<std::string, std::string> & data);
        enum class ConnectionState { None, WaitForOkMessageAfterSendStatus };

        Peers::STATE GetState() { return workerState; }
        ConnectionState GetConnectionState() { return workerConnectionState; }
        unsigned int GetMaxThread() { return maxThread; }
        void SetState(const Peers::STATE state) { workerState = state; }
        void SetConnectionState(ConnectionState state) { workerConnectionState = state; }
        void SetMaxThread(const unsigned int val) { maxThread = val; }

    private:
        Peers::STATE workerState;        
        ConnectionState workerConnectionState = ConnectionState::None;
        unsigned int maxThread;
};

#endif // WORKER_H
