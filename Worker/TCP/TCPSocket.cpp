#include "TCPSocket.h"

TCPSocket::TCPSocket() : socket_(io_service_global),
                         resolver(io_service_global)
{
    Traces() << "\n" << "LOG: TCPSocket::TCPSocket(const std::string &adress, const std::string &port)";
    meWsk = TCPSocket_ptr(this);
}

void TCPSocket::Close()
{
  io_service_global.post(boost::bind(&TCPSocket::DoClose, this));
}

void TCPSocket::Connect(const std::string &adress, const std::string &port)
{
    Traces() << "\n" << "LOG: void TCPSocket::Connect(const std::string &adress, const std::string &port)";    

    tcp::resolver::query query(adress, port);
    querywsk = new tcp::resolver::query(" ", " ");

    *querywsk = query;
    iterator = resolver.resolve(*querywsk);

     boost::asio::async_connect(socket_, iterator,
           boost::bind(&TCPSocket::HandleConnect, this,
           boost::asio::placeholders::error));

     thread_io_service = boost::thread(boost::bind(&boost::asio::io_service::run, &io_service_global));
}

void TCPSocket::HandleConnect(const boost::system::error_code& error)
{
  Traces() << "\n" << "LOG: void TCPSocket::HandleConnect(const boost::system::error_code& error)";

  if (!error)
  {
     Traces() << "\n" << "LOG: Read data";

     Message tempMessage;
     char *buffer = new char[MessageCoder::MaxMessageConnectedSize()];
     MessageCoder::ClearChar(buffer, MessageCoder::MaxMessageConnectedSize());
     MessageCoder::CreateConnectedMessage(buffer);
     tempMessage.CopyWsk(meWsk, buffer);
     messageQueue->PushBack(tempMessage);

     data_to_read = new char[MessageCoder::MaxMessageSize()];
     MessageCoder::ClearChar(data_to_read, MessageCoder::MaxMessageSize());

     boost::asio::async_read(socket_,
        boost::asio::buffer(data_to_read, MessageCoder::MaxMessageSize()), boost::asio::transfer_all(),
        boost::bind(&TCPSocket::HandleConnect, this,
          boost::asio::placeholders::error));

     Traces() << "\n" << "LOG: Message received: " << std::string(data_to_read);
  } else
  {
        Traces() << "\n" << "ERR: Connection error!";
        DoClose();
  }
}


void TCPSocket::WriteMessage(char *dataToSend)
{
    io_service_global.post(boost::bind(&TCPSocket::Write, this, dataToSend));
}

void TCPSocket::Write(char *dataToSend)
{
    Traces() << "\n" << "LOG: void TCPSocket::Write(char *dataToSend)";

    data = dataToSend;

    boost::asio::async_write(socket_,
        boost::asio::buffer(data, std::strlen(data)),
        boost::bind(&TCPSocket::HandleWrite, this,
          boost::asio::placeholders::error));

}

void TCPSocket::HandleWrite(const boost::system::error_code& error)
{
  Traces() << "\n" << "LOG: void TCPSocket::HandleWrite(const boost::system::error_code& error)";

  if (!error)
  {

      /*boost::asio::async_write(socket_,
          boost::asio::buffer(data,
            std::strlen(data)),
          boost::bind(&TCPSocket::HandleWrite, this,
            boost::asio::placeholders::error));*/
  }
  else
  {
        Traces() << "\n" << "ERR: Read error!";
        DoClose();
  }
}

void TCPSocket::DoClose()
{
  Message tempMessage;
  char *buffer = new char[MessageCoder::MaxMessageConnectionCloseSize()];
  MessageCoder::ClearChar(buffer, MessageCoder::MaxMessageConnectionCloseSize());
  MessageCoder::CreateCloseConnectionMessage(buffer);
  tempMessage.CopyWsk(meWsk, buffer);
  messageQueue->PushBack(tempMessage);

  socket_.close();
}
