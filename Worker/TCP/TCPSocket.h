#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>

using namespace std;
using boost::asio::ip::tcp;

class TCPSocket
{
    public:
        TCPSocket(boost::asio::io_service& io_service,
                  tcp::resolver::iterator endpoint_iterator);

    private:
      void HandleConnect(const boost::system::error_code& error);
      void HandleReadHeader(const boost::system::error_code& error);

      char *data;
      boost::asio::io_service& io_service_;
      tcp::socket socket_;

};

#endif // TCPSOCKET_H
