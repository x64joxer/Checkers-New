#include "TCPSocket.h"

TCPSocket::TCPSocket(const std::string &adress, const std::string &port)
                     : socket_(io_service),
                       resolver(io_service)
{

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
