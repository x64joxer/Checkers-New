#include "TCPConnection.h"
#include <boost/bind.hpp>

TCPConnection::TCPConnection(boost::asio::io_service& io_service)
                            : socket_(io_service),
                              socketActive(true)
{
    Traces() << "\n" << "LOG: TCPConnection::TCPConnection(boost::asio::io_service& io_service) : socket_(io_service)";
}

void TCPConnection::Start()
{
  Traces() << "\n" << "LOG: void TCPConnection::Start()";

  socket_.async_read_some(boost::asio::buffer(buffer_),
      boost::bind(&TCPConnection::HandleRead, shared_from_this(),
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));
}

void TCPConnection::HandleRead(const boost::system::error_code& e,
    std::size_t bytes_transferred)
{
  Traces() << "\n" << "LOG: void TCPConnection::HandleRead(const boost::system::error_code& e, std::size_t bytes_transferred)";

  if (!e)
  {
    Traces() << "\n" << "LOG: New message";
    std::cout << buffer_.data() << std::endl;
  }
  else if (e != boost::asio::error::operation_aborted)
  {
    Traces() << "\n" << "LOG: Close connection";
    this->Stop();
  }
}

void TCPConnection::HandleWrite(const boost::system::error_code& e)
{
  Traces() << "\n" << "LOG: void TCPConnection::HandleWrite(const boost::system::error_code& e)";

  if (!e)
  {
    // Initiate graceful connection closure.
    boost::system::error_code ignored_ec;
    socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
  }

  if (e != boost::asio::error::operation_aborted)
  {
    //TO_DO Send error signal
  }
}


void TCPConnection::Stop()
{
  Traces() << "\n" << "LOG: void TCPConnection::Stop()";

  socketActive = false;
  socket_.close();
}

boost::asio::ip::tcp::socket& TCPConnection::Socket()
{
  Traces() << "\n" << "LOG: boost::asio::ip::tcp::socket& TCPConnection::Socket()";

  return socket_;
}
