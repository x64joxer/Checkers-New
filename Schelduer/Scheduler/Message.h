#ifndef MESSAGE_H
#define MESSAGE_H


#include <boost/shared_ptr.hpp>
#include <cstring>
#include "../Traces/Traces.h"

class  TCPConnection;

typedef boost::shared_ptr<TCPConnection> TCPConnection_ptr;

class Message
{
    public:         
        Message();
        Message(const Message & data);

        void CopyData(TCPConnection_ptr connection, const char *wskM);
        void CopyWsk(TCPConnection_ptr connection, char *wskM);
        char *GetWskMessage() { return wskMessage; }
        TCPConnection_ptr GetTCPConnection_ptr() { return connectionWsk; }

        Message & operator=(const Message  & data);
        ~Message();

    private:
        TCPConnection_ptr connectionWsk;
        char *wskMessage = nullptr;
};

#endif // MESSAGE_H
