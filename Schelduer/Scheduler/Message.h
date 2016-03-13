#ifndef MESSAGE_H
#define MESSAGE_H


#include <boost/shared_ptr.hpp>

class  TCPConnection;

typedef boost::shared_ptr<TCPConnection> TCPConnection_ptr;

class Message
{
    public:
        void CopyData(TCPConnection_ptr connection, const char *wskM)
        {
            wskMessage = new char[std::strlen(wskM)];
            std::strcpy(wskMessage, wskM);
            connectionWsk = connection;
        }

        Message & operator=(Message & data)
        {
            connectionWsk = data.connectionWsk;

            delete [] wskMessage;
            wskMessage = new char[std::strlen(data.wskMessage)];
            std::strcpy(wskMessage, data.wskMessage);

            return *this;
        }

    public:
        TCPConnection_ptr connectionWsk;
        char *wskMessage;
};

#endif // MESSAGE_H
