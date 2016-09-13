#include "TCPServer.h"

#include <boost/bind.hpp>
#include <signal.h>

TCPServer::TCPServer(const std::string& address, const std::string& port)
                                                    : io_service_(),
                                                    acceptor_(io_service_),
                                                    new_connection_(),
                                                    connectionManagerWSk(nullptr)

{
    Traces() << "\n" << "LOG: TCPServer::TCPServer(const std::string& address, const std::string& port)";

    boost::asio::ip::tcp::resolver resolver(io_service_);
    boost::asio::ip::tcp::resolver::query query(address, port);
    boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);
    acceptor_.open(endpoint.protocol());
    acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    acceptor_.bind(endpoint);
    acceptor_.listen();

    StartAccept();
}

void TCPServer::Run()
{
  Traces() << "\n" << "LOG: void TCPServer::Run()";
  io_service_.run();
}

void TCPServer::StartAccept()
{
  Traces() << "\n" << "LOG: void TCPServer::StartAccept()";

  new_connection_.reset(new TCPConnection(io_service_));
  acceptor_.async_accept(new_connection_->Socket(),
      boost::bind(&TCPServer::HandleAccept, this,
        boost::asio::placeholders::error()));
}

void TCPServer::HandleAccept(const boost::system::error_code& e)
{
  Traces() << "\n" << "LOG: void TCPServer::HandleAccept(const boost::system::error_code& e)";

  if (!acceptor_.is_open())
  {
    return;
  }

  if (!e)
  {
    Traces() << "\n" << "LOG: Adding new connection to ConnectionManager";
    connectionManagerWSk->NewConnection(new_connection_);
    new_connection_->SetMeWsk(new_connection_);
    new_connection_.reset();
  }

  StartAccept();
}
