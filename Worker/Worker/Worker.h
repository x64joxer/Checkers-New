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
#include "../Counters/Counters.h"

class Worker
{
    public:
        Worker();
        void Start();

        ~Worker();

    private:
        void StartWorking();
        void ReceiveJob(TCPSocket_ptr socket, std::map<std::string, std::string> & data, char * dest, QueueTimer & reconnectionTimer, bool fast = false);
        void SendRegisterMessage(TCPSocket_ptr socket, char * dest, std::string & prevousMessageid);
        void SendStateMessage(TCPSocket_ptr socket, char * dest, std::string & prevousMessageid);
        void SendBestResultWhenJobEnd(Board & board, char * dest, std::string & jobId, QueueTimer & reconnectionTimer);
        void SendResult(Board & board, char * dest, std::string & jobId, QueueTimer & reconnectionTimer);
        void SendCanNotMoveMessage(char * dest, std::string & jobId, QueueTimer & reconnectionTimer);
        void ReceiveStopAnalyse(TCPSocket_ptr socket, std::map<std::string, std::string> & data, char * dest);

        void MessageInterpreting(TCPSocket_ptr socket, std::map<std::string, std::string> & data, char * dest, QueueTimer & reconnectionTimer, std::string & prevousMessageid);
        unsigned int CalculateMaxTimeForIA(const unsigned int maxTime, const unsigned int reservedTime);

        std::thread workerThread;
        unsigned int maxThread;
        TCPSocket socketToServer;
        SharedPtrList<Message> *messageQueue;
        std::condition_variable *condition_var;
        enum ConState { DISCONNECTED, CONNECTED, REGISTERED, STATEUPDATED, BEST_RESULT_SEND, BEST_RESULT_FAST_SEND, CAN_NOT_MOVE_SEND } connection_state;
        Peers::STATE myState;
        bool conversationIsOngoing;
        std::string prevousMessageid;

        unsigned int maxIaTime;
        unsigned int numOfResultToReturnFast;
        Board boardToAnalyse;
        std::string jobId;
        std::thread iaJob;
        std::atomic_bool endIaJobFlag;
        std::atomic<bool> canITakeBoardToReturnFast;
        bool firstWorker;
        std::atomic<int> currentPercentOfSteps;
        ThreadIAMove<3000000> *jobExpander;
        std::atomic_bool stopFlag;        
        std::atomic_bool canNotMoveFlag;

        /////////////////////////
        //Traffic test purposes
        /////////////////////////
        char *destTraffic = new char[MessageCoder::MaxMessageSize()];
        unsigned long long previousTimeOfClearCounternumberOfBestResultMessagesSend;
        /////////////////////////
        //END traffic test purposes
        /////////////////////////

};

#endif // WORKER_H
