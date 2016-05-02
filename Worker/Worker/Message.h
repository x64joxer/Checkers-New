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

        Message() : wskMessage(nullptr), connectionWsk(nullptr) {}

        Message(const Message & data)
        {
            Traces() << "\n" << "LOG: Message(const Message & data)";

            connectionWsk = data.connectionWsk;

            if (data.wskMessage)
            {
                wskMessage = new char[std::strlen(data.wskMessage)];
                std::strcpy(wskMessage, data.wskMessage);
            } else
            {
                wskMessage = nullptr;
            }

            Traces() << "\n" << "LOG: Number of connectionWsk = " << connectionWsk.use_count();
        }

        ~Message()
        {
            Traces() << "\n" << "LOG: ~Message()";

            delete [] wskMessage;
            wskMessage = nullptr;

            Traces() << "\n" << "LOG: Number of connectionWsk = " << connectionWsk.use_count();
        }

        void CopyData(TCPSocket_ptr connection, const char *wskM)
        {
            Traces() << "\n" << "LOG: void CopyData(TCPConnection_ptr connection, const char *wskM)";

            wskMessage = new char[std::strlen(wskM)];
            std::strcpy(wskMessage, wskM);
            connectionWsk = connection;

            Traces() << "\n" << "LOG: Number of connectionWsk = " << connectionWsk.use_count();
        }        

        void CopyWsk(TCPSocket_ptr connection, char *wskM)
        {
            Traces() << "\n" << "LOG: void CopyWsk(TCPSocket_ptr connection, const char *wskM)";

            wskMessage = wskM;
            connectionWsk = connection;

            Traces() << "\n" << "LOG: Number of connectionWsk = " << connectionWsk.use_count();
        }       

        char *GetWskMessage() { return wskMessage; }
        TCPSocket_ptr GetTCPSocket_ptr() { return connectionWsk; }

        Message & operator=(const Message  & data)
        {
            Traces() << "\n" << "LOG: Message & operator=(const Message & data)";

            if (this != &data)
            {
                connectionWsk = data.connectionWsk;
                delete [] wskMessage;

                if (data.wskMessage)
                {
                    wskMessage = new char[std::strlen(data.wskMessage)];
                    std::strcpy(wskMessage, data.wskMessage);
                } else
                {
                    wskMessage = nullptr;
                }
            }

            Traces() << "\n" << "LOG: Number of connectionWsk = " << connectionWsk.use_count();

            return *this;
        }

    private:
        TCPSocket_ptr connectionWsk;
        char *wskMessage;

};

#endif // MESSAGE_H
