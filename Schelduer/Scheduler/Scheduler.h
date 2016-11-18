#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <thread>
#include <iostream>
#include <map>
#include <mutex>
#include <sstream>
#include "../Traces/Traces.h"
#include "../ConnectionManager/ConnectionManager.h"
#include "MessageCoder.h"
#include "Client.h"
#include "../Counters/Counters.h"
#include "Worker.h"
#include "../SafeQueue/SharedMap/SharedMap.h"
#include "../SafeQueue/SharedPtrSet/SharedPtrList.h"
#include "../ServerState/ServerState.h"
#include "../QueueTimer/QueueTimer.h"
#include "../QueueTimer/QueueTimerList.h"
#include "../ProgramVariables/ProgramVariables.h"

class Scheduler
{
    public:
        Scheduler();

        ConnectionManager * GetConnectionManager();        
        void Start(const unsigned short numofthread);
        void SetConnectionManager(ConnectionManager *wsk);        
        ~Scheduler();

    private:
        void StartScheduling();
        void MessageInterpreting(TCPConnection_ptr socket, std::map<std::string, std::string> & data, char * dest);

        void AddClient(TCPConnection_ptr socket, const std::map<std::string, std::string> & data, char * dest);
        void AddWorker(TCPConnection_ptr socket, const std::map<std::string, std::string> & data, char * dest);
        void SetRole(TCPConnection_ptr socket, const std::map<std::string, std::string> & data, char * dest);
        void SendStopAnalyse(TCPConnection_ptr socket, const std::string & messageId, char * dest);
        void SetState(TCPConnection_ptr socket, const std::map<std::string, std::string> & data, char * dest);
        void SetState(TCPConnection_ptr socket, Peers::STATE state);
        void SendServerState(TCPConnection_ptr socket, const ServerState & serverState, const std::string & messageId, char * dest);
        void SendServerState(TCPConnection_ptr socket, const ServerState & serverState, const std::map<std::string, std::string> & data, char * dest);
        bool RemoveClient(TCPConnection_ptr socket);
        bool RemoveWorker(TCPConnection_ptr socket);
        void ResetServerState(TCPConnection_ptr socket, const std::map<std::string, std::string> & data, char * dest);
        void CreateTimeoutGuard(TCPConnection_ptr socket, const unsigned int miliseconds);
        void CreateTimeToSendResultToClientsGuard(TCPConnection_ptr socket, const unsigned int miliseconds);
        void UpdateFreeWorkerList(TCPConnection_ptr & socket, Worker_ptr worker);
        void DistributeWorkToWorkers(char * dest);
        void UpdateNextClientStatus(TCPConnection_ptr tmpTCP_Connection_ptr, char * dest);
        void SendStopAnalyseToWorker(TCPConnection_ptr tmpTCP_Connection_ptr, char * dest);
        void RecevieBestResult(TCPConnection_ptr socket, const std::map<std::string, std::string> & data, char * dest);
        void FinishWork(const std::map<std::string, std::string> & data, char * dest);
        void SendStateToAllClients(const std::map<std::string, std::string> & data, char * dest);
        Board CalculateBestResult();

        ConnectionManager *wskConnectionManager;

        std::thread schedulerThread[16];
        std::mutex mutex;
        std::mutex mutexWorkDistribute;
        std::condition_variable *condition_var;
        SharedMap<TCPConnection_ptr, Client_ptr> clients;
        SharedMap<TCPConnection_ptr, Worker_ptr> workers;

        SharedPtrList<TCPConnection_ptr> freeWorkers;
        SharedPtrList<TCPConnection_ptr> clientsToStateUpdate;
        SharedPtrList<TCPConnection_ptr> workersToStopAnalyse;
        SharedPtrList<Board> boardsToAnalyse;        
        std::atomic<bool> firstJobStarted;        
        std::atomic<bool> workOngoing;        

        ServerState state;
        QueueTimerList timerList;
        QueueTimer jobTimer;
};

#endif // SCHEDULER_H
