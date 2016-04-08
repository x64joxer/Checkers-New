#include "TCPSocket.h"

TCPSocket::TCPSocket(const std::string &adress, const std::string &port, boost::asio::io_service & serviceio)
                     : io_service(serviceio),
                       socket_(io_service),
                       resolver(io_service)

{
 Traces() << "\n" << "LOG: TCPSocket::TCPSocket(const std::string &adress, const std::string &port)";

 data_to_read = new char[100];

 tcp::resolver::query query(adress, port);
 querywsk = new tcp::resolver::query(" ", " ");



 *querywsk = query;
 iterator = resolver.resolve(*querywsk);

  boost::asio::async_connect(socket_, iterator,
        boost::bind(&TCPSocket::HandleConnect, this,
        boost::asio::placeholders::error));

}

void TCPSocket::HandleConnect(const boost::system::error_code& error)
{
  Traces() << "\n" << "LOG: void TCPSocket::HandleConnect(const boost::system::error_code& error)";

  if (!error)
  {
     Traces() << "\n" << "LOG: Read data";

     boost::asio::async_read(socket_,
        boost::asio::buffer(data_to_read, std::strlen(data)),
        boost::bind(&TCPSocket::HandleConnect, this,
          boost::asio::placeholders::error));

     Traces() << "\n" << "LOG: Message received: " << std::string(data_to_read);
  } else
  {
        Traces() << "\n" << "ERR:";
  }
}


void TCPSocket::WriteMessage(char *dataToSend)
{
    io_service.post(boost::bind(&TCPSocket::Write, this, dataToSend));
}

void TCPSocket::Write(char *dataToSend)
{
    Traces() << "\n" << "LOG: void TCPSocket::Write(char *dataToSend)";

    data = dataToSend;

    boost::asio::async_write(socket_,
        boost::asio::buffer(data, std::strlen(data)),
        boost::bind(&TCPSocket::HandleWrite, this,
          boost::asio::placeholders::error));

}

void TCPSocket::HandleWrite(const boost::system::error_code& error)
{
  Traces() << "\n" << "LOG: void TCPSocket::HandleWrite(const boost::system::error_code& error)";

  if (!error)
  {

      /*boost::asio::async_write(socket_,
          boost::asio::buffer(data,
            std::strlen(data)),
          boost::bind(&TCPSocket::HandleWrite, this,
            boost::asio::placeholders::error));*/
  }
  else
  {
        Traces() << "\n" << "ERR:";
  }
}

