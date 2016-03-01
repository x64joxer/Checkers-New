#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H


#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

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
     boost::asio::ip::tcp::socket& Socket();

   private:
      boost::asio::ip::tcp::socket socket_;

       /// Buffer for incoming data.
       boost::array<char, 8192> buffer_;
};

typedef boost::shared_ptr<TCPConnection> TCPConnection_ptr;

#endif // TCPCONNECTION_H
