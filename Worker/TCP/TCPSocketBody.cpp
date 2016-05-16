#include "TCPSocketBody.h"

TCPSocketBody::TCPSocketBody() : socket_(io_service_global),
                                 resolver(io_service_global),
                                 connected(false)
{
    Traces() << "\n" << "LOG: TCPSocketBody::TCPSocketBody(const std::string &adress, const std::string &port)";    
}

void TCPSocketBody::Close()
{
  io_service_global.post(boost::bind(&TCPSocketBody::DoClose, this));
}

void TCPSocketBody::Connect(const std::string &adress, const std::string &port)
{
    Traces() << "\n" << "LOG: void TCPSocketBody::Connect(const std::string &adress, const std::string &port)";

    tcp::resolver::query query(adress, port);
    querywsk = new tcp::resolver::query(" ", " ");

    *querywsk = query;
    iterator = resolver.resolve(*querywsk);    

     boost::asio::async_connect(socket_, iterator,
           boost::bind(&TCPSocketBody::HandleConnect, this,
           boost::asio::placeholders::error));     

     socket_.set_option(boost::asio::ip::tcp::no_delay(true));

     thread_io_service = boost::thread(boost::bind(&boost::asio::io_service::run, &io_service_global));
}

void TCPSocketBody::HandleConnect(const boost::system::error_code& error)
{
  Traces() << "\n" << "LOG: void TCPSocketBody::HandleConnect(const boost::system::error_code& error)";

  if (!error)
  {           
     bool tmpFlag = connected;

     if (!connected)
     {
         connected = true;

         Message tempMessage;
         char *buffer = new char[MessageCoder::MaxMessageConnectedSize()];
         MessageCoder::ClearChar(buffer, MessageCoder::MaxMessageConnectedSize());
         MessageCoder::CreateConnectedMessage(buffer);
         tempMessage.CopyWsk(meWsk, buffer);
         messageQueue->PushBack(tempMessage);

         data_to_read = new char[MessageCoder::MaxMessageSize()];
         MessageCoder::ClearChar(data_to_read, MessageCoder::MaxMessageSize());
     } else
     {
         Traces() << "\n" << "LOG: Read data";
     }


     boost::asio::async_read(socket_,
        boost::asio::buffer(data_to_read,10), boost::asio::transfer_all(),
        boost::bind(&TCPSocketBody::HandleConnect, this,
          boost::asio::placeholders::error));

     boost::asio::async_read(socket_,
        boost::asio::buffer(data_to_read,40), boost::asio::transfer_all(),
        boost::bind(&TCPSocketBody::HandleConnect, this,
          boost::asio::placeholders::error));

     if (tmpFlag)
     {
         Traces() << "\n" << "LOG: Message received: " << std::string(data_to_read);

         Message tempMessage;
         tempMessage.CopyWsk(meWsk, data_to_read);
         messageQueue->PushBack(tempMessage);
     }


  } else
  {
        Traces() << "\n" << "ERR: Connection error!";
        Close();
  }
}


void TCPSocketBody::WriteMessage(char *dataToSend)
{
    io_service_global.post(boost::bind(&TCPSocketBody::Write, this, dataToSend));
}

void TCPSocketBody::Write(char *dataToSend)
{
    Traces() << "\n" << "LOG: void TCPSocketBody::Write(char *dataToSend)";

    data = dataToSend;

    boost::asio::async_write(socket_,
        boost::asio::buffer(data, std::strlen(data)),
        boost::bind(&TCPSocketBody::HandleWrite, this,
          boost::asio::placeholders::error));

}

void TCPSocketBody::HandleWrite(const boost::system::error_code& error)
{
  Traces() << "\n" << "LOG: void TCPSocketBody::HandleWrite(const boost::system::error_code& error)";

  if (!error)
  {

      /*boost::asio::async_write(socket_,
          boost::asio::buffer(data,
            std::strlen(data)),
          boost::bind(&TCPSocketBody::HandleWrite, this,
            boost::asio::placeholders::error));*/
  }
  else
  {
        Traces() << "\n" << "ERR: Read error!";
        Close();
  }
}

void TCPSocketBody::DoClose()
{
  socket_.close();

  connected = false;

  Message tempMessage;
  char *buffer = new char[MessageCoder::MaxMessageConnectionCloseSize()];
  MessageCoder::ClearChar(buffer, MessageCoder::MaxMessageConnectionCloseSize());
  MessageCoder::CreateCloseConnectionMessage(buffer);
  tempMessage.CopyWsk(meWsk, buffer);
  messageQueue->PushBack(tempMessage);  
}
