#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <thread>
#include <iostream>
#include <map>
#include <mutex>
#include "../Traces/Traces.h"
#include "../ConnectionManager/ConnectionManager.h"
#include "MessageCoder.h"
#include "Client.h"
#include "Worker.h"
#include "../ProgramVariables/ProgramVariables.h"
#include "../SafeQueue/SharedMap/SharedMap.h"
#include "../SafeQueue/SharedPtrSet/SharedPtrList.h"
#include "../ServerState/ServerState.h"
#include "../QueueTimer/QueueTimer.h"
#include "../QueueTimer/QueueTimerList.h"

class Scheduler
{
    public:
        Scheduler();

        ConnectionManager * GetConnectionManager();
        void MessageInterpreting(TCPConnection_ptr socket, std::map<std::string, std::string> & data, char * dest);
        void Start(const unsigned short numofthread);
        void SetConnectionManager(ConnectionManager *wsk);        
        ~Scheduler();

    private:
        void StartScheduling();

        void AddClient(TCPConnection_ptr socket, const std::map<std::string, std::string> & data, char * dest);
        void AddWorker(TCPConnection_ptr socket, const std::map<std::string, std::string> & data, char * dest);
        void SetRole(TCPConnection_ptr socket, const std::map<std::string, std::string> & data, char * dest);
        void SetState(TCPConnection_ptr socket, const std::map<std::string, std::string> & data, char * dest);
        void SendServerState(TCPConnection_ptr socket, const ServerState & serverState, const std::string & messageId, char * dest);
        void SendServerState(TCPConnection_ptr socket, const ServerState & serverState, const std::map<std::string, std::string> & data, char * dest);        
        bool RemoveClient(TCPConnection_ptr socket);
        bool RemoveWorker(TCPConnection_ptr socket);
        void CreateTimeoutGuard(TCPConnection_ptr socket, const unsigned int miliseconds);
        void CreateTimeToSendResultToClientsGuard(TCPConnection_ptr socket, const unsigned int miliseconds);
        void UpdateFreeWorkerList(TCPConnection_ptr & socket, Worker_ptr worker);
        void DistributeWorkToWorkers(char * dest);
        void UpdateNextClientStatus(TCPConnection_ptr tmpTCP_Connection_ptr, char * dest);
        void RecevieBestResult(TCPConnection_ptr socket, const std::map<std::string, std::string> & data, char * dest);
        void SendStateToAllClients(const std::map<std::string, std::string> & data, char * dest);

        ConnectionManager *wskConnectionManager;

        std::thread schedulerThread[16];
        std::mutex mutex;
        std::condition_variable *condition_var;
        SharedMap<TCPConnection_ptr, Client_ptr> clients;
        SharedMap<TCPConnection_ptr, Worker_ptr> workers;

        SharedPtrList<TCPConnection_ptr> freeWorkers;
        SharedPtrList<TCPConnection_ptr> clientsToStateUpdate;
        SharedPtrList<Board> boardsToAnalyse;        
        std::atomic<bool> firstJobStarted;        

        ServerState state;
        QueueTimerList timerList;
};

#endif // SCHEDULER_H
