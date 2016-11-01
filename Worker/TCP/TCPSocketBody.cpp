#include "TCPSocketBody.h"

TCPSocketBody::TCPSocketBody() : connected(false),
                                 readThreadStarted(false),
                                 expectedMessage(0)
{
    TRACE_FLAG_FOR_CLASS_TCPSocketBody Traces() << "\n" << "LOG: TCPSocketBody::TCPSocketBody(const std::string &adress, const std::string &port)";

    data_to_read = new char[MessageCoder::MaxMessageSize()];
}

void TCPSocketBody::Close()
{
    TRACE_FLAG_FOR_CLASS_TCPSocketBody Traces() << "\n" << "LOG: void TCPSocketBody::Close()";

    DoClose();
}

void TCPSocketBody::Connect(const std::string &adress, const std::string &port)
{
    TRACE_FLAG_FOR_CLASS_TCPSocketBody Traces() << "\n" << "LOG: void TCPSocketBody::Connect(const std::string &adress, const std::string &port)";

    sf::Socket::Status status = socket.connect(adress, atoi(port.c_str()));
    if (status != sf::Socket::Done)
    {
        Traces() << "\n" << "ERR: connection error!";

        Close();
    } else
    {
        Message tempMessage;
        char *buffer = new char[MessageCoder::MaxMessageConnectedSize()];
        MessageCoder::ClearChar(buffer, MessageCoder::MaxMessageConnectedSize());
        MessageCoder::CreateConnectedMessage(buffer);
        tempMessage.CopyWsk(meWsk, buffer);
        messageQueue->PushBack(tempMessage);

        socket.setBlocking(false);
        read_thread = std::move(std::thread(&TCPSocketBody::HandleRead,
                                            this));
        read_thread.detach();
        readThreadStarted = true;
        connected = true;
    }
}

void TCPSocketBody::WriteMessage(char *dataToSend)
{
    TRACE_FLAG_FOR_CLASS_TCPSocketBody Traces() << "\n" << "LOG: void TCPSocketBody::WriteMessage(char *dataToSend)";

    if (connected)
    {
        if (socket.send(dataToSend, strlen(dataToSend)) != sf::Socket::Done)
        {
            Traces() << "\n" << "ERR: Write message eroor!";
            Close();
        }
    } else
    {
        Traces() << "\n" << "ERR: Socket not connected!";
    }
}

void TCPSocketBody::HandleRead()
{
    TRACE_FLAG_FOR_CLASS_TCPSocketBody Traces() << "\n" << "LOG: void TCPSocketBody::HandleRead()";

    sf::SocketSelector selector;
    selector.add(socket);

    std::size_t received;
    expectedMessage = 0;
    bool closeSocker = false;

    while(connected)
    {
        if (selector.wait(sf::milliseconds(milisecondCloseSocketInterval)))
        {
            if (connected)
            {
                if (expectedMessage == 0)
                {
                    if (socket.receive(data_to_read, MessageCoder::BufferSize(), received) != sf::Socket::Done)
                    {
                        Traces() << "\n" << "ERR: Read header error";
                        closeSocker = true;
                        break;
                    } else
                    {
                        expectedMessage = MessageCoder::HeaderToVal(data_to_read);

                        if (expectedMessage > MessageCoder::MaxMessageSize())
                        {
                            Traces() << "\n" << "ERR: Message to long";

                            closeSocker = true;
                            break;
                        }
                    }
                } else
                {
                    if (socket.receive(data_to_read, expectedMessage, received) != sf::Socket::Done)
                    {
                        Traces() << "\n" << "ERR: Read message error";
                        closeSocker = true;
                        break;
                    } else
                    {
                        if (expectedMessage != received)
                        {
                            Traces() << "\n" << "ERR: Message to long or to short";

                            closeSocker = true;
                            break;
                        } else
                        {
                            expectedMessage = 0;

                            Message tempMessage;
                            tempMessage.CopyData(meWsk, data_to_read);
                            messageQueue->PushBack(tempMessage);
                        }
                    }
                }
            }
        }
    }

    readThreadStarted = false;
    if (closeSocker) Close();
}

void TCPSocketBody::DoClose()
{
  TRACE_FLAG_FOR_CLASS_TCPSocketBody Traces() << "\n" << "LOG: void TCPSocketBody::DoClose()";

  if (connected) socket.disconnect();
  connected = false;
  while (readThreadStarted) {}

  Message tempMessage;
  char *buffer = new char[MessageCoder::MaxMessageConnectionCloseSize()];
  MessageCoder::ClearChar(buffer, MessageCoder::MaxMessageConnectionCloseSize());
  MessageCoder::CreateCloseConnectionMessage(buffer);
  tempMessage.CopyWsk(meWsk, buffer);
  messageQueue->PushBack(tempMessage);  

}

TCPSocketBody::~TCPSocketBody()
{
    delete [] data_to_read;
}
