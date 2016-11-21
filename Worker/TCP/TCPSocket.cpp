#include "TCPSocket.h"

TCPSocket::TCPSocket() : messageQueue(nullptr)
{
    TRACE_FLAG_FOR_CLASS_TCPSocket Traces() << "\n" << "LOG: TCPSocket::TCPSocket(const std::string &adress, const std::string &port)";
    meWsk = TCPSocket_ptr(this);

    bodySocket.SetMessageQueue(messageQueue);
    bodySocket.SetMyWsk(meWsk);
}

void TCPSocket::Close()
{
  bodySocket.Close();
}

void TCPSocket::Connect(const std::string &adress, const std::string &port)
{
    TRACE_FLAG_FOR_CLASS_TCPSocket Traces() << "\n" << "LOG: void TCPSocket::Connect(const std::string &adress, const std::string &port)";

    Close();
    bodySocket.Connect(adress,port);
}

void TCPSocket::WriteMessage(char *dataToSend)
{
    bodySocket.WriteMessage(dataToSend);
}


TCPSocket::~TCPSocket()
{    
  TRACE_FLAG_FOR_CLASS_TCPSocket Traces() << "\n" << "LOG:  TCPSocket::~TCPSocket()";

}
