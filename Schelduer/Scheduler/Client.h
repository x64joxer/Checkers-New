#ifndef CLIENT_H
#define CLIENT_H

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

#endif // CLIENT_H
