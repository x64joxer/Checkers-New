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
#include "TCPSocketBody.h"

using namespace std;
using boost::asio::ip::tcp;

class TCPSocket
{
    public:
        TCPSocket();
        ~TCPSocket();
        void Close();
        void Connect(const std::string &adress, const std::string &port);
        void SetMessageQueue(SharedPtrList<Message> *message) { messageQueue = message; bodySocket.SetMessageQueue(message); }
        void WriteMessage(char *dataToSend);
        TCPSocket_ptr GeetMyWsk() { return meWsk; }

    private:
      void HandleConnect(const boost::system::error_code& error);      

      TCPSocketBody bodySocket;
      TCPSocket_ptr meWsk;
      SharedPtrList<Message> *messageQueue;      
};

#endif // TCPSOCKET_H
