#include "TCPConnection.h"
#include <boost/bind.hpp>

TCPConnection::TCPConnection(boost::asio::io_service& io_service)
                            : socket_(io_service),
                              socketActive(true),
                              io_service_(io_service)
{
    TRACE_FLAG_FOR_CLASS_TCPConnection Traces() << "\n" << "LOG: TCPConnection::TCPConnection(boost::asio::io_service& io_service) : socket_(io_service)";

    finalBuffer = new char[MessageCoder::MaxMessageConnectionCloseSize()];
    finalBuffer[0] = 0;
    bytesInFinalBuffer = 0;
}

void TCPConnection::Start()
{
  TRACE_FLAG_FOR_CLASS_TCPConnection Traces() << "\n" << "LOG: void TCPConnection::Start()";
 socket_.set_option( boost::asio::ip::tcp::no_delay( true) );
 socket_.set_option( boost::asio::socket_base::send_buffer_size( 65536 ) );
 socket_.set_option( boost::asio::socket_base::receive_buffer_size( 65536 ) );

  socket_.async_read_some(boost::asio::buffer(buffer_),
      boost::bind(&TCPConnection::HandleRead, shared_from_this(),
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));
}

void TCPConnection::Close()
{
  TRACE_FLAG_FOR_CLASS_TCPConnection Traces() << "\n" << "LOG: void TCPConnection::Close()";

  io_service_.post(boost::bind(&TCPConnection::Stop, this));
}

void TCPConnection::HandleRead(const boost::system::error_code& e,
    std::size_t bytes_transferred)
{
  TRACE_FLAG_FOR_CLASS_TCPConnection Traces() << "\n" << "LOG: void TCPConnection::HandleRead(const boost::system::error_code& e, std::size_t bytes_transferred)";

  if (!e)
  {
    TRACE_FLAG_FOR_CLASS_TCPConnection Traces() << "\n" << "LOG: New message from " << socket_.remote_endpoint().address().to_string() << ":" << socket_.remote_endpoint().port();

    strcat(finalBuffer, buffer_.data());
    bytesInFinalBuffer += bytes_transferred;
    finalBuffer[bytesInFinalBuffer] = 0;

    if (strchr(finalBuffer, '\n') != NULL)
    {
        Message tempMessage;
        tempMessage.CopyData(meWsk, finalBuffer);
        TRACE_FLAG_FOR_CLASS_TCPConnection Traces() << "\n" << "LOG: Push back new message: " << finalBuffer;
        messageQueue->PushBack(tempMessage);

        finalBuffer[0] = 0;
        bytesInFinalBuffer = 0;
    } else
    {
        TRACE_FLAG_FOR_CLASS_TCPConnection Traces() << "\n" << "LOG: final buffer contains: " << finalBuffer;
    }


    socket_.async_read_some(boost::asio::buffer(buffer_),
        boost::bind(&TCPConnection::HandleRead, shared_from_this(),
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred));
  }
  else if (e != boost::asio::error::operation_aborted)
  {
    TRACE_FLAG_FOR_CLASS_TCPConnection Traces() << "\n" << "LOG: Close connection";

    socketActive = false;
    this->Close();
  }
}

void TCPConnection::HandleWrite(const boost::system::error_code& e)
{
  TRACE_FLAG_FOR_CLASS_TCPConnection Traces() << "\n" << "LOG: void TCPConnection::HandleWrite(const boost::system::error_code& e)";

  sendMutex.unlock();

  if (!e)
  {
    // Initiate graceful connection closure.
    //boost::system::error_code ignored_ec;
    //socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
  }

  if (e != boost::asio::error::operation_aborted)
  {
    //TO_DO Send error signal
  }
}

void TCPConnection::SendMessage(const std::string & message)
{
    TRACE_FLAG_FOR_CLASS_TCPConnection Traces() << "\n" << "LOG: void TCPConnection::SendMessage(const std::string message)";

    sendMutex.lock();

    TRACE_FLAG_FOR_CLASS_TCPConnection Traces() << "\n" << "LOG: Sending message: " << message;

    boost::asio::async_write(socket_, boost::asio::buffer(message),
              boost::bind(&TCPConnection::HandleWrite, shared_from_this(),
                boost::asio::placeholders::error));
}

void TCPConnection::Stop()
{
  TRACE_FLAG_FOR_CLASS_TCPConnection Traces() << "\n" << "LOG: void TCPConnection::Stop()";

  if (socketActive)
  {
      socket_.shutdown(socket_.shutdown_both);
      socket_.close();
  }

  socketActive = false;

  Message tempMessage;
  char *buffer = new char[MessageCoder::MaxMessageConnectionCloseSize()];
  MessageCoder::ClearChar(buffer, MessageCoder::MaxMessageConnectionCloseSize());
  MessageCoder::CreateCloseConnectionMessage(buffer);
  tempMessage.CopyWsk(meWsk, buffer);
  meWsk.reset();
  messageQueue->PushBack(tempMessage);
}

boost::asio::ip::tcp::socket& TCPConnection::Socket()
{
  TRACE_FLAG_FOR_CLASS_TCPConnection Traces() << "\n" << "LOG: boost::asio::ip::tcp::socket& TCPConnection::Socket()";

  return socket_;
}


TCPConnection::~TCPConnection()
{
    TRACE_FLAG_FOR_CLASS_TCPConnection Traces() << "\n" << "LOG: TCPConnection::~TCPConnection()";

    delete [] finalBuffer;
}
