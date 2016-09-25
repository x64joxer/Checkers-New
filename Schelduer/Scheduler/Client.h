#ifndef CLIENT_H
#define CLIENT_H

#include <boost/shared_ptr.hpp>
#include "../Traces/Traces.h"

class Client
{
    public:
        Client();

        enum class ConnectionState { None, WaitForOkMessageAfterSendStatus };
        ConnectionState GetConnectionState() { return workerConnectionState; }
        void SetConnectionState(ConnectionState state) { workerConnectionState = state; }

    private:
        ConnectionState workerConnectionState = ConnectionState::None;
};

typedef boost::shared_ptr<Client> Client_ptr;

#endif // CLIENT_H
