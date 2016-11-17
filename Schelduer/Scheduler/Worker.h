#ifndef WORKER_H
#define WORKER_H

#include <boost/shared_ptr.hpp>
#include <map>
#include "../Peers/Peers.h"
#include "../Traces/Traces.h"
#include "MessageCoder.h"
#include "../ProgramVariables/ProgramVariables.h"

class Worker
{
    public:
        Worker();
        Worker(const std::map<std::string, std::string> & data);
        enum class ConnectionState { None, WaitForOkMessageAfterSendStatus, WaitForOkMessageAfterSendFirstJob, WaitForOkMessageAfterSendJob, WaitForOkMessageAfterSendStopAnnalyse };

        Peers::STATE GetState() { return workerState; }
        ConnectionState GetConnectionState() { return workerConnectionState; }
        unsigned int GetMaxThread() { return maxThread; }
        void SetState(const Peers::STATE state) { workerState = state; }
        void SetConnectionState(ConnectionState state) { workerConnectionState = state; }
        void SetMaxThread(const unsigned int val) { maxThread = val; }

        ~Worker();
    private:
        Peers::STATE workerState;        
        ConnectionState workerConnectionState = ConnectionState::None;
        unsigned int maxThread;
};

typedef boost::shared_ptr<Worker> Worker_ptr;

#endif // WORKER_H
