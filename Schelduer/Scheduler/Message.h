#ifndef MESSAGE_H
#define MESSAGE_H


#include <boost/shared_ptr.hpp>
#include "../Traces/Traces.h"

class  TCPConnection;

typedef boost::shared_ptr<TCPConnection> TCPConnection_ptr;

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

        void CopyData(TCPConnection_ptr connection, const char *wskM)
        {
            Traces() << "\n" << "LOG: void CopyData(TCPConnection_ptr connection, const char *wskM)";

            wskMessage = new char[std::strlen(wskM)];
            std::strcpy(wskMessage, wskM);
            connectionWsk = connection;
        }        

        void CopyWsk(TCPConnection_ptr connection, char *wskM)
        {
            Traces() << "\n" << "LOG: void CopyWsk(TCPSocket_ptr connection, const char *wskM)";

            wskMessage = wskM;
            connectionWsk = connection;
        }

        char *GetWskMessage() { return wskMessage; }
        TCPConnection_ptr GetTCPConnection_ptr() { return connectionWsk; }

        Message & operator=(const Message  & data)
        {
            Traces() << "\n" << "LOG: Message & operator=(const Message & data)";

            connectionWsk = data.connectionWsk;

            delete [] wskMessage;
            wskMessage = new char[std::strlen(data.wskMessage)];
            std::strcpy(wskMessage, data.wskMessage);

            return *this;
        }

    private:
        TCPConnection_ptr connectionWsk;
        char *wskMessage = nullptr;
};

#endif // MESSAGE_H
