#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <iostream>
#include <thread>
#include <mutex>
#include <boost/asio/ip/tcp.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include "../Traces/Traces.h"
#include "../Worker/MessageCoder.h"
#include "../Worker/Message.h"
#include "../SafeQueue/SharedPtrSet/SharedPtrList.h"

using namespace std;
using boost::asio::ip::tcp;

class TCPSocket
{
    public:
        TCPSocket();
        void Close();
        void Connect(const std::string &adress, const std::string &port);
        void WriteMessage(char *dataToSend);

    private:
      void DoClose();
      void HandleConnect(const boost::system::error_code& error);      
      void HandleWrite(const boost::system::error_code& error);
      void Write(char *dataToSend);

      char *data;
      char *data_to_read;      
      SharedPtrList<Message> *messageQueue;

      boost::thread thread_io_service;
      boost::asio::io_service io_service_global;

      tcp::socket socket_;
      tcp::resolver resolver;
      tcp::resolver::iterator iterator;
      tcp::resolver::query *querywsk;      
};

#endif // TCPSOCKET_H
