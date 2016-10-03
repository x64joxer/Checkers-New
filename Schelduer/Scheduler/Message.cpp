#include "Message.h"

Message::Message()
{

}

Message::Message(const Message & data)
{
    Traces() << "\n" << "LOG: Message(const Message & data)";

    connectionWsk = data.connectionWsk;
    wskMessage = data.wskMessage;

}

void Message::CopyData(TCPConnection_ptr connection, const char *wskM)
{
    Traces() << "\n" << "LOG: void CopyData(TCPConnection_ptr connection, const char *wskM)";

    char *tmpChar = new char[std::strlen(wskM)];
    std::strcpy(tmpChar, wskM);

    wskMessage.reset(tmpChar);
    connectionWsk = connection;
}

void Message::CopyWsk(TCPConnection_ptr connection, char *wskM)
{
    Traces() << "\n" << "LOG: void CopyWsk(TCPSocket_ptr connection, const char *wskM)";

    wskMessage.reset(wskM);
    connectionWsk = connection;
}

Message & Message::operator=(const Message  & data)
{
    Traces() << "\n" << "LOG: Message & operator=(const Message & data)";

    if (this != &data)
    {
        connectionWsk = data.connectionWsk;
        wskMessage = data.wskMessage;
    }

    return *this;
}

Message::~Message()
{
    Traces() << "\n" << "LOG: ~Message()";    
}
