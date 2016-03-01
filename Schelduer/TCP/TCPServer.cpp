#include "TCPServer.h"

#include <boost/bind.hpp>
#include <signal.h>

TCPServer::TCPServer(const std::string& address, const std::string& port)
                                                    : io_service_(),
                                                    acceptor_(io_service_),
                                                    new_connection_(),
                                                    schedulerWSk(nullptr)

{
    // Open the acceptor with the option to reuse the address (i.e. SO_REUSEADDR).
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
  // The io_service::run() call will block until all asynchronous operations
  // have finished. While the server is running, there is always at least one
  // asynchronous operation outstanding: the asynchronous accept call waiting
  // for new incoming connections.
  io_service_.run();
}

void TCPServer::StartAccept()
{
  new_connection_.reset(new TCPConnection(io_service_));
  acceptor_.async_accept(new_connection_->Socket(),
      boost::bind(&TCPServer::HandleAccept, this,
        boost::asio::placeholders::error()));
}

void TCPServer::HandleAccept(const boost::system::error_code& e)
{
  // Check whether the server was stopped by a signal before this completion
  // handler had a chance to run.
  if (!acceptor_.is_open())
  {
    return;
  }

  if (!e)
  {
    std::cout << "yea" << std::endl;
    schedulerWSk->NewConnection(new_connection_);
  }

  StartAccept();
}
