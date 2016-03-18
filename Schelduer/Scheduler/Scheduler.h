#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <thread>
#include <map>
#include <mutex>
#include "../Traces/Traces.h"
#include "../ConnectionManager/ConnectionManager.h"
#include "MessageCoder.h"
#include "Client.h"
#include "../SafeQueue/SharedMap/SharedMap.h"
#include "../ServerState/ServerState.h"

class Scheduler
{
    public:
        Scheduler();

        ConnectionManager * GetConnectionManager();
        void MessageInterpreting(TCPConnection_ptr socket, std::map<std::string, std::string> & dest);
        void Start(const unsigned short numofthread);
        void SetConnectionManager(ConnectionManager *wsk);        
        ~Scheduler();

    private:
        void StartScheduling();

        void AddClient(TCPConnection_ptr socket, const std::map<std::string, std::string> & data);
        void SetRole(TCPConnection_ptr socket, const std::map<std::string, std::string> & data);
        void SendServerState(TCPConnection_ptr socket, const ServerState & serverState, const std::map<std::string, std::string> & data);

        ConnectionManager *wskConnectionManager;

        std::thread schedulerThread[8];
        std::mutex mutex;
        std::condition_variable *condition_var;
        SharedMap<TCPConnection_ptr, Client> clients;

        ServerState state;
};

#endif // SCHEDULER_H
