#include "TCPSocketBody.h"

TCPSocketBody::TCPSocketBody() : socket_(io_service_global),
                                 resolver(io_service_global),
                                 connected(false),
                                 expectedMessage(0)
{
    Traces() << "\n" << "LOG: TCPSocketBody::TCPSocketBody(const std::string &adress, const std::string &port)";    
}

void TCPSocketBody::Close()
{
  Traces() << "\n" << "LOG: void TCPSocketBody::Close()";

  io_service_global.post(boost::bind(&TCPSocketBody::DoClose, this));
}

void TCPSocketBody::Connect(const std::string &adress, const std::string &port)
{
    Traces() << "\n" << "LOG: void TCPSocketBody::Connect(const std::string &adress, const std::string &port)";

    io_service_global.reset();

    iterator = resolver.resolve({adress, port});

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
      connected = true;

      Message tempMessage;
      char *buffer = new char[MessageCoder::MaxMessageConnectedSize()];
      MessageCoder::ClearChar(buffer, MessageCoder::MaxMessageConnectedSize());
      MessageCoder::CreateConnectedMessage(buffer);
      tempMessage.CopyWsk(meWsk, buffer);
      messageQueue->PushBack(tempMessage);

      Traces() << "\n" << "LOG: Sending Connected message to queue: " << buffer;      

      boost::asio::async_read(socket_,
         boost::asio::buffer(data_to_read, MessageCoder::BufferSize()), boost::asio::transfer_all(),
         boost::bind(&TCPSocketBody::HandleReadHeader, this,
           boost::asio::placeholders::error));

  } else
  {
        Traces() << "\n" << "ERR: Connection error!";
        Close();
  }
}

void TCPSocketBody::HandleReadHeader(const boost::system::error_code& error)
{
    Traces() << "\n" << "LOG: void TCPSocketBody::HandleReadHeader(const boost::system::error_code& error)";

    expectedMessage = MessageCoder::HeaderToVal(data_to_read);
    delete [] data_to_read;

    Traces() << "\n" << "LOG: Expecting message with lenn: " << expectedMessage;

    if (expectedMessage > MessageCoder::MaxMessageSize())
    {
       Traces() << "\n" << "ERR: Protocol error. Message too long.";

       expectedMessage = 0;
       Close();
    } else
    {
        boost::asio::async_read(socket_,
           boost::asio::buffer(data_to_read, expectedMessage), boost::asio::transfer_all(),
           boost::bind(&TCPSocketBody::HandleReadMessage, this,
             boost::asio::placeholders::error));
    }
}

void TCPSocketBody::HandleReadMessage(const boost::system::error_code& error)
{
    Traces() << "\n" << "LOG: void TCPSocketBody::HandleReadMessage(const boost::system::error_code& error)";

    Traces() << "\n" << "LOG: Message received: " << std::string(data_to_read);

    Message tempMessage;
    tempMessage.CopyWsk(meWsk, data_to_read);
    messageQueue->PushBack(tempMessage);
    expectedMessage = 0;        

    boost::asio::async_read(socket_,
       boost::asio::buffer(data_to_read, MessageCoder::BufferSize()), boost::asio::transfer_all(),
       boost::bind(&TCPSocketBody::HandleReadHeader, this,
         boost::asio::placeholders::error));
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
  Traces() << "\n" << "LOG: void TCPSocketBody::DoClose()";

  socket_.close();  
  connected = false;
  Message tempMessage;
  char *buffer = new char[MessageCoder::MaxMessageConnectionCloseSize()];
  MessageCoder::ClearChar(buffer, MessageCoder::MaxMessageConnectionCloseSize());
  MessageCoder::CreateCloseConnectionMessage(buffer);
  tempMessage.CopyWsk(meWsk, buffer);
  messageQueue->PushBack(tempMessage);  

}
