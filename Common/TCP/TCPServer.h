#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <boost/asio.hpp>
#include <string>
#include <boost/noncopyable.hpp>
#include "TCPConnection.h"

class TCPServer
{
    public:
        TCPServer(const std::string& address, const std::string& port);
         void Run();
         void SetConnectionHandler(TCPConnection_ptr wsk) { connectionHandler = wsk;}

    private:
        void HandleAccept(const boost::system::error_code& e);
        void StartAccept();

        boost::asio::io_service io_service_;
        /// Acceptor used to listen for incoming connections.
        boost::asio::ip::tcp::acceptor acceptor_;
        /// The next connection to be accepted.
        TCPConnection_ptr new_connection_;

        void (*connectionHandler)(TCPConnection_ptr);
};

#endif // TCPSERVER_H
