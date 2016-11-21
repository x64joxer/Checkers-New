#ifndef CLIENT_H
#define CLIENT_H

#include <boost/shared_ptr.hpp>
#include "../Traces/Traces.h"
#include "../ProgramVariables/ProgramVariables.h"

class Client
{
    public:
        Client();

        enum class ConnectionState { None, WaitForOkMessageAfterSendStatus };
        ConnectionState GetConnectionState() const { return workerConnectionState; }
        std::string GetLastMessageID() const { return lastMessageId; }
        void SetConnectionState(ConnectionState state) { workerConnectionState = state; }
        void SetLastMessageID(const std::string & id)  { lastMessageId = id; }

    private:
        ConnectionState workerConnectionState = ConnectionState::None;
        std::string lastMessageId;
};

typedef boost::shared_ptr<Client> Client_ptr;

#endif // CLIENT_H
