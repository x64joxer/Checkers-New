#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H


#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "../Traces/Traces.h"
#include "../SafeQueue/SharedPtrSet/SharedPtrList.h"
#include "../SafeQueue/SharedPtrSet/SharedPtrSet.h"

class  TCPConnection;

//Helpful types

typedef boost::shared_ptr<TCPConnection> TCPConnection_ptr;

struct Message
{
    void CopyData(TCPConnection * connection, const char *wskM)
    {
        wskMessage = new char[std::strlen(wskM)];
        std::strcpy(wskMessage, wskM);
        connectionWsk = connection;
    }

    Message & operator=(Message & data)
    {
        connectionWsk = data.connectionWsk;

        delete [] wskMessage;
        wskMessage = new char[std::strlen(data.wskMessage)];
        std::strcpy(wskMessage, data.wskMessage);

        return *this;
    }


    TCPConnection *connectionWsk;
    char *wskMessage;
};

//Main class

class TCPConnection : public boost::enable_shared_from_this<TCPConnection>,
                      private boost::noncopyable
{
   public:
     /// Construct a connection with the given io_service.
     explicit TCPConnection(boost::asio::io_service& io_service);

     void Start();
     void Stop();
     void HandleRead(const boost::system::error_code& e, std::size_t bytes_transferred);
     void HandleWrite(const boost::system::error_code& e);
     void SendMessage(const std::string message);
     void SetMessageQueue(SharedPtrList<Message> * wsk) { messageQueue = wsk; }
     bool IsSocketActive() { return socketActive; }

     boost::asio::ip::tcp::socket& Socket();

   private:     
     SharedPtrList<Message> *messageQueue;
     boost::array<char, 8192> buffer_;
     boost::asio::ip::tcp::socket socket_;
     bool socketActive;
};

#endif // TCPCONNECTION_H
