#include "Message.h"

Message::Message() : wskMessage(nullptr),
                     connectionWsk(nullptr)
{

}

Message::Message(const Message & data)
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

void Message::CopyData(TCPSocket_ptr connection, const char *wskM)
{
    Traces() << "\n" << "LOG: void CopyData(TCPConnection_ptr connection, const char *wskM)";

    wskMessage = new char[std::strlen(wskM)];
    std::strcpy(wskMessage, wskM);
    connectionWsk = connection;

    Traces() << "\n" << "LOG: Number of connectionWsk = " << connectionWsk.use_count();
}

void Message::CopyWsk(TCPSocket_ptr connection, char *wskM)
{
    Traces() << "\n" << "LOG: void CopyWsk(TCPSocket_ptr connection, const char *wskM)";

    delete [] wskMessage;
    wskMessage = wskM;
    connectionWsk = connection;

    Traces() << "\n" << "LOG: Number of connectionWsk = " << connectionWsk.use_count();
}

Message & Message::operator=(const Message  & data)
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

Message::~Message()
{
    Traces() << "\n" << "LOG: ~Message()";

    delete [] wskMessage;    

    Traces() << "\n" << "LOG: Number of connectionWsk = " << connectionWsk.use_count();
}
