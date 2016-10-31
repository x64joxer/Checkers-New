#ifndef TCPSOCKEDBODY_H
#define TCPSOCKEDBODY_H

#include <iostream>
#include <thread>
#include <mutex>
#include <boost/asio/ip/tcp.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <SFML/Network.hpp>
#include "../Traces/Traces.h"
#include "../Worker/MessageCoder.h"
#include "../Worker/Message.h"
#include "../SafeQueue/SharedPtrSet/SharedPtrList.h"
#include "../ProgramVariables/ProgramVariables.h"

class TCPSocket;

typedef boost::shared_ptr<TCPSocket> TCPSocket_ptr;

using namespace std;
using boost::asio::ip::tcp;

class TCPSocketBody
{
    public:
        TCPSocketBody();
        void Close();
        void Connect(const std::string &adress, const std::string &port);
        void SetMessageQueue(SharedPtrList<Message> *message) { messageQueue = message; }
        void WriteMessage(char *dataToSend);       
        void SetMyWsk(TCPSocket_ptr wsk) { meWsk = wsk; }
        bool IsConneted() { return connected; }        
        ~TCPSocketBody();

    private:
      void DoClose();           
      void HandleRead();

      char *data;
      char *data_to_read;      
      SharedPtrList<Message> *messageQueue;
      TCPSocket_ptr meWsk;
      bool connected;
      bool readThreadStarted;

      std::thread read_thread;
      unsigned int expectedMessage;      

      sf::TcpSocket socket;
      unsigned int milisecondCloseSocketInterval = 250;
};

#endif // TCPSOCKEDBODY_H
