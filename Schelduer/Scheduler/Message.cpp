#include "Message.h"

Message::Message()
{

}

Message::Message(const Message & data)
{
    Traces() << "\n" << "LOG: Message(const Message & data)";

    connectionWsk = data.connectionWsk;

    delete [] wskMessage;
    wskMessage = new char[std::strlen(data.wskMessage)];
    std::strcpy(wskMessage, data.wskMessage);
}

void Message::CopyData(TCPConnection_ptr connection, const char *wskM)
{
    Traces() << "\n" << "LOG: void CopyData(TCPConnection_ptr connection, const char *wskM)";

    wskMessage = new char[std::strlen(wskM)];
    std::strcpy(wskMessage, wskM);
    connectionWsk = connection;
}

void Message::CopyWsk(TCPConnection_ptr connection, char *wskM)
{
    Traces() << "\n" << "LOG: void CopyWsk(TCPSocket_ptr connection, const char *wskM)";

    wskMessage = wskM;
    connectionWsk = connection;
}

Message & Message::operator=(const Message  & data)
{
    Traces() << "\n" << "LOG: Message & operator=(const Message & data)";

    connectionWsk = data.connectionWsk;

    delete [] wskMessage;
    wskMessage = new char[std::strlen(data.wskMessage)];
    std::strcpy(wskMessage, data.wskMessage);

    return *this;
}

Message::~Message()
{
    Traces() << "\n" << "LOG: ~Message()";
    delete [] wskMessage;
    wskMessage = nullptr;
}
