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
#include "../Scheduler/Message.h"

class TCPConnection : public boost::enable_shared_from_this<TCPConnection>,
                      private boost::noncopyable
{
   public:
     /// Construct a connection with the given io_service.
     explicit TCPConnection(boost::asio::io_service& io_service);

     void Start();
     void Stop();
     TCPConnection_ptr GetMeWsk() { return meWsk; }
     void HandleRead(const boost::system::error_code& e, std::size_t bytes_transferred);
     void HandleWrite(const boost::system::error_code& e);
     void SendMessage(const std::string message);
     void SetMessageQueue(SharedPtrList<Message> * wsk) { messageQueue = wsk; }
     void SetMeWsk(TCPConnection_ptr me) { meWsk = me; }
     bool IsSocketActive() { return socketActive; }

     boost::asio::ip::tcp::socket& Socket();

   private:     
     SharedPtrList<Message> *messageQueue;
     boost::array<char, 8192> buffer_;
     TCPConnection_ptr meWsk;
     boost::asio::ip::tcp::socket socket_;
     bool socketActive;
};

#endif // TCPCONNECTION_H
