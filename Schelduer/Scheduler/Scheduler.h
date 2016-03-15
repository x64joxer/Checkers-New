#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <thread>
#include <map>
#include <mutex>
#include "../Traces/Traces.h"
#include "../ConnectionManager/ConnectionManager.h"
#include "MessageCoder.h"
#include "Client.h"

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

        ConnectionManager *wskConnectionManager;

        std::thread schedulerThread[8];
        std::mutex mutex;
        std::condition_variable *condition_var;
        std::map<TCPConnection_ptr, Client> clients;
};

#endif // SCHEDULER_H
