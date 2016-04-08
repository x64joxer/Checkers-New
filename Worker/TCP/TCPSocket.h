#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <iostream>
#include <thread>
#include <boost/asio/ip/tcp.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include "../Traces/Traces.h"

using namespace std;
using boost::asio::ip::tcp;

class TCPSocket
{
    public:
        TCPSocket(const std::string &adress, const std::string &port);
        void WriteMessage(char *dataToSend);
        void Write(char *dataToSend);
        void HandleWrite(const boost::system::error_code& error);

    private:
      void HandleConnect(const boost::system::error_code& error);      

      char *data;
      char *data_to_read;

      tcp::socket socket_;
      tcp::resolver resolver;
      tcp::resolver::iterator iterator;
      tcp::resolver::query *querywsk;

      boost::thread thread_io_service;

      static boost::asio::io_service io_service_global;
      static bool io_service_thread_active;
};

#endif // TCPSOCKET_H
