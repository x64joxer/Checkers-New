#ifndef WORKER_H
#define WORKER_H

#include "../Peers/Peers.h"

class Worker
{
    public:
        Worker();

        Peers::STATE GetState() { return workerState; }
        unsigned int GetMaxThread() { return maxThread; }
        void SetState(const Peers::STATE state) { workerState = state; }
        void SetMaxThread(const unsigned int val) { maxThread = val; }

    private:
        Peers::STATE workerState;
        unsigned int maxThread;
};

#endif // WORKER_H
