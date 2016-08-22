#include "TCPSocket.h"

TCPSocket::TCPSocket() : bodySocket (nullptr)
{
    Traces() << "\n" << "LOG: TCPSocket::TCPSocket(const std::string &adress, const std::string &port)";
    meWsk = TCPSocket_ptr(this);

    bodySocket = new TCPSocketBody();
    bodySocket->SetMessageQueue(messageQueue);
    bodySocket->SetMyWsk(meWsk);
}

void TCPSocket::Close()
{
  bodySocket->Close();
}

void TCPSocket::Connect(const std::string &adress, const std::string &port)
{
    Traces() << "\n" << "LOG: void TCPSocket::Connect(const std::string &adress, const std::string &port)";    

    if (bodySocket != nullptr)
    {
        if (bodySocket->IsConneted()) bodySocket->Close();

        delete bodySocket;
        bodySocket = new TCPSocketBody();
        bodySocket->SetMessageQueue(messageQueue);
        bodySocket->SetMyWsk(meWsk);
    }

    bodySocket->Connect(adress,port);

}

void TCPSocket::HandleConnect(const boost::system::error_code& error)
{
  Traces() << "\n" << "LOG: void TCPSocket::HandleConnect(const boost::system::error_code& error)";

}


void TCPSocket::WriteMessage(char *dataToSend)
{
    bodySocket->WriteMessage(dataToSend);   
}


TCPSocket::~TCPSocket()
{
  Traces() << "\n" << "LOG:  TCPSocket::~TCPSocket()";
}
