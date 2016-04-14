#ifndef MESSAGE_H
#define MESSAGE_H


#include <boost/shared_ptr.hpp>
#include "../Traces/Traces.h"

class  TCPSocket;

typedef boost::shared_ptr<TCPSocket> TCPSocket_ptr;

class Message
{
    public:         

        Message() {}

        Message(const Message & data)
        {
            Traces() << "\n" << "LOG: Message(const Message & data)";

            connectionWsk = data.connectionWsk;

            delete [] wskMessage;
            wskMessage = new char[std::strlen(data.wskMessage)];
            std::strcpy(wskMessage, data.wskMessage);
        }

        ~Message()
        {
            Traces() << "\n" << "LOG: ~Message()" << wskMessage;
            delete [] wskMessage;
            wskMessage = nullptr;
        }

        void CopyData(TCPSocket_ptr connection, const char *wskM)
        {
            Traces() << "\n" << "LOG: void CopyData(TCPConnection_ptr connection, const char *wskM)";

            wskMessage = new char[std::strlen(wskM)];
            std::strcpy(wskMessage, wskM);
            connectionWsk = connection;
        }        

        Message & operator=(const Message  & data)
        {
            Traces() << "\n" << "LOG: Message & operator=(const Message & data)";

            connectionWsk = data.connectionWsk;

            delete [] wskMessage;
            wskMessage = new char[std::strlen(data.wskMessage)];
            std::strcpy(wskMessage, data.wskMessage);

            return *this;
        }

    public:
        TCPSocket_ptr connectionWsk;
        char *wskMessage = nullptr;
};

#endif // MESSAGE_H
