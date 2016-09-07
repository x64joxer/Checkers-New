#ifndef WORKER_H
#define WORKER_H

#include "../Peers/Peers.h"

class Worker
{
    public:
        Worker();
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
