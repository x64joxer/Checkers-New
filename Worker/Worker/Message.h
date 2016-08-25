#ifndef MESSAGE_H
#define MESSAGE_H

#include <cstring>
#include <boost/shared_ptr.hpp>
#include "../Traces/Traces.h"

class  TCPSocket;

typedef boost::shared_ptr<TCPSocket> TCPSocket_ptr;

class Message
{
    public:         

        Message();
        Message(const Message & data);
        void CopyData(TCPSocket_ptr connection, const char *wskM);
        void CopyWsk(TCPSocket_ptr connection, char *wskM);
        char *GetWskMessage() { return wskMessage; }
        TCPSocket_ptr GetTCPSocket_ptr() { return connectionWsk; }
        Message & operator=(const Message  & data);
        ~Message();

    private:
        TCPSocket_ptr connectionWsk;
        char *wskMessage;

};

#endif // MESSAGE_H
