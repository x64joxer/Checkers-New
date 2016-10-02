#ifndef WORKER_H
#define WORKER_H

#include <thread>
#include <mutex>
#include "../TCP/TCPSocket.h"
#include "MessageCoder.h"
#include "../ProgramVariables/ProgramVariables.h"
#include "../QueueTimer/QueueTimer.h"
#include "../Peers/Peers.h"
#include "../IA/IAPossibleMoves.h"
#include "../ThreadIA/ThreadIAMove.h"

class Worker
{
    public:
        Worker();
        void Start();

        ~Worker();

    private:
        void StartWorking();
        void ReceiveJob(TCPSocket_ptr socket, std::map<std::string, std::string> & data, char * dest, QueueTimer & reconnectionTimer, std::string & prevousMessageid, bool fast = false);
        void SendRegisterMessage(TCPSocket_ptr socket, char * dest, std::string & prevousMessageid);
        void SendStateMessage(TCPSocket_ptr socket, char * dest, std::string & prevousMessageid);

        void MessageInterpreting(TCPSocket_ptr socket, std::map<std::string, std::string> & data, char * dest, QueueTimer & reconnectionTimer, std::string & prevousMessageid);
        void SendBestResultWhenJobEnd(Board & board, char * dest, std::string & prevousMessageid, std::string & jobId, QueueTimer & reconnectionTimer);

        unsigned int CalculateMaxTimeForIA(const unsigned int maxTime, const unsigned int reservedTime);

        std::thread workerThread;
        unsigned int maxThread;
        TCPSocket socketToServer;
        SharedPtrList<Message> *messageQueue;
        std::condition_variable *condition_var;
        enum ConState { DISCONNECTED, CONNECTED, REGISTERED, STATEUPDATED, BEST_RESULT_SEND } connection_state;
        Peers::STATE myState;

        unsigned int maxIaTime;
        unsigned int numOfResultToReturnFast;
        Board boardToAnalyse;
        std::string jobId;
        std::thread iaJob;
        std::atomic_bool endIaJobFlag;
        bool firstWorker;
        std::atomic<int> currentPercentOfSteps;
        ThreadIAMove<3000000> jobExpander;

};

#endif // WORKER_H
