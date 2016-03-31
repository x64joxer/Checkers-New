#include "TCPSocket.h"

TCPSocket::TCPSocket(boost::asio::io_service& io_service,
                       tcp::resolver::iterator endpoint_iterator)
                     : io_service_(io_service),
                       socket_(io_service)
{

  boost::asio::async_connect(socket_, endpoint_iterator,
        boost::bind(&TCPSocket::HandleConnect, this,
        boost::asio::placeholders::error));


}

void TCPSocket::HandleConnect(const boost::system::error_code& error)
{
  if (!error)
  {
     boost::asio::async_read(socket_,
        boost::asio::buffer(data, 100),
        boost::bind(&TCPSocket::HandleReadHeader, this,
          boost::asio::placeholders::error));
  } else
  {
      int *i;
      i = new int();
      delete i;
      delete i;
  }
}

void TCPSocket::HandleReadHeader(const boost::system::error_code& error)
{
  if (!error)
  {      
  //  boost::asio::async_read(socket_,
  //      boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
  //      boost::bind(&chat_client::handle_read_body, this,
  //        boost::asio::placeholders::error));
 }
}
