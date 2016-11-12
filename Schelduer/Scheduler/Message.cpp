#include "Message.h"

Message::Message()
{

}

Message::Message(const Message & data)
{
    TRACE_FLAG_FOR_CLASS_Message Traces() << "\n" << "LOG: Message(const Message & data)";

    connectionWsk = data.connectionWsk;
    wskMessage = data.wskMessage;

}

void Message::CopyData(TCPConnection_ptr connection, const char *wskM)
{
    TRACE_FLAG_FOR_CLASS_Message Traces() << "\n" << "LOG: void CopyData(TCPConnection_ptr connection, const char *wskM)";

    char *tmpChar = new char[std::strlen(wskM)+1];
    std::strcpy(tmpChar, wskM);

    wskMessage.reset(tmpChar);
    connectionWsk = connection;
}

void Message::CopyWsk(TCPConnection_ptr connection, char *wskM)
{
    TRACE_FLAG_FOR_CLASS_Message Traces() << "\n" << "LOG: void CopyWsk(TCPSocket_ptr connection, const char *wskM)";

    wskMessage.reset(wskM);
    connectionWsk = connection;
}

Message & Message::operator=(const Message  & data)
{
    TRACE_FLAG_FOR_CLASS_Message Traces() << "\n" << "LOG: Message & operator=(const Message & data)";

    if (this != &data)
    {
        connectionWsk = data.connectionWsk;
        wskMessage = data.wskMessage;
    }

    return *this;
}

Message::~Message()
{
    TRACE_FLAG_FOR_CLASS_Message Traces() << "\n" << "LOG: ~Message()";
}
