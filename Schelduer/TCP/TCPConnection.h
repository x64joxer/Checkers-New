#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H


#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <mutex>
#include "../Traces/Traces.h"
#include "../SafeQueue/SharedPtrSet/SharedPtrList.h"
#include "../SafeQueue/SharedPtrSet/SharedPtrSet.h"
#include "../Scheduler/Message.h"
#include "../Scheduler/MessageCoder.h"
#include "../ProgramVariables/ProgramVariables.h"

class TCPConnection : public boost::enable_shared_from_this<TCPConnection>,
                      private boost::noncopyable
{
   public:
     /// Construct a connection with the given io_service.
     explicit TCPConnection(boost::asio::io_service& io_service);

     void Start();
     void Close();
     TCPConnection_ptr GetMeWsk() { return meWsk; }
     std::string GetIp() { return socket_.remote_endpoint().address().to_string(); }
     int GetPort() { return socket_.remote_endpoint().port(); }
     void SendMessage(const std::string message);    
     void SetMessageQueue(SharedPtrList<Message> * wsk) { messageQueue = wsk; }
     void SetMeWsk(TCPConnection_ptr me) { meWsk = me; }
     bool IsSocketActive() { return socketActive; }

     boost::asio::ip::tcp::socket& Socket();
     ~TCPConnection();

   private:
     void HandleRead(const boost::system::error_code& e, std::size_t bytes_transferred);
     void HandleWrite(const boost::system::error_code& e);
     void Stop();
     SharedPtrList<Message> *messageQueue;
     boost::array<char, 8192> buffer_;
     TCPConnection_ptr meWsk;     
     boost::asio::ip::tcp::socket socket_;
     boost::asio::io_service& io_service_;
     bool socketActive;
     std::mutex sendMutex;
};

#endif // TCPCONNECTION_H
