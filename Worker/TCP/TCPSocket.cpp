#include "TCPSocket.h"

TCPSocket::TCPSocket() : messageQueue(nullptr)
{
    Traces() << "\n" << "LOG: TCPSocket::TCPSocket(const std::string &adress, const std::string &port)";
    meWsk = TCPSocket_ptr(this);

    bodySocket.SetMessageQueue(messageQueue);
    bodySocket.SetMyWsk(meWsk);
}

void TCPSocket::Close()
{
  if (bodySocket.IsConneted()) bodySocket.Close();
}

void TCPSocket::Connect(const std::string &adress, const std::string &port)
{
    Traces() << "\n" << "LOG: void TCPSocket::Connect(const std::string &adress, const std::string &port)";    

    Close();
    bodySocket.Connect(adress,port);
}

void TCPSocket::HandleConnect(const boost::system::error_code& error)
{
  Traces() << "\n" << "LOG: void TCPSocket::HandleConnect(const boost::system::error_code& error)";

}


void TCPSocket::WriteMessage(char *dataToSend)
{
    bodySocket.WriteMessage(dataToSend);
}


TCPSocket::~TCPSocket()
{    
  Traces() << "\n" << "LOG:  TCPSocket::~TCPSocket()";

}
