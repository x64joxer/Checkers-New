#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <iostream>
#include <boost/asio/ip/tcp.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>

using namespace std;
using boost::asio::ip::tcp;

class TCPSocket
{
    public:
        TCPSocket(const std::string &adress, const std::string &port);

    private:
      void HandleConnect(const boost::system::error_code& error);
      void HandleReadHeader(const boost::system::error_code& error);


      char *data;



      boost::asio::io_service io_service;
      tcp::socket socket_;
      tcp::resolver resolver;
      tcp::resolver::iterator iterator;
      tcp::resolver::query *querywsk;


};

#endif // TCPSOCKET_H
