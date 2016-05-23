#include "Worker.h"

Worker::Worker() : connection_state(DISCONNECTED),
                   myState(Peers::STATE::FREE),
                   maxThread(1)
{
    Traces() << "\n" << "LOG: Worker::Worker()";

    messageQueue = new SharedPtrList<Message>;
}

void Worker::Start()
{
    Traces() << "\n" << "LOG: void Worker::Start()";

    socketToServer.SetMessageQueue(messageQueue);
    socketToServer.Connect("192.168.0.7", "6000");

    workerThread = std::move(std::thread(&Worker::StartWorking,
                                          this));

    workerThread.join();
}

void Worker::StartWorking()
{
    Traces() << "\n" << "LOG: void Worker::StartWorking()";

    std::string prevousMessageid;

    condition_var = messageQueue->GetCondVar();
    char *dest = new char[MessageCoder::MaxMessageSize()];

    Message tmpMessage;

    QueueTimer reconnectionTimer;
    Message timeoutMessage;

    char *tmpChar = new char[MessageCoder::MaxMessageTimeoutSize()];
    MessageCoder::ClearChar(tmpChar, MessageCoder::MaxMessageTimeoutSize());
    MessageCoder::CreateTimeoutMessage(tmpChar);
    timeoutMessage.CopyWsk(socketToServer.GeetMyWsk(), tmpChar);

    reconnectionTimer.SetTime(5000);
    reconnectionTimer.SetMessageToSend(timeoutMessage);
    reconnectionTimer.SetQueue(messageQueue);

    while(true)
    {
        Traces() << "\n" << "LOG: Waiting for a job..";
        std::mutex tmpMutex;
        std::unique_lock<std::mutex> guard(tmpMutex);

        condition_var->wait(guard,[this]
        {
            return !messageQueue->Empty();
        }
        );

        tmpMessage = messageQueue->PopFront();

        std::map<std::string, std::string> messageContent;
        MessageCoder::MessageToMap(tmpMessage.GetWskMessage(), messageContent);

        MessageInterpreting(tmpMessage.GetTCPSocket_ptr(), messageContent, dest, reconnectionTimer, prevousMessageid);
    }

    delete [] dest;
}

void Worker::MessageInterpreting(TCPSocket_ptr socket, std::map<std::string, std::string> & data, char * dest, QueueTimer & reconnectionTimer, std::string & prevousMessageid)
{
    Traces() << "\n" << "LOG: void Worker::MessageInterpreting(TCPSocket_ptr socket, std::map<std::string, std::string> & data, char * dest, QueueTimer & reconnectionTimer, std::string & prevousMessageid)";

    try
    {
        std::string action = data.at(MessageCoder::ACTION);

        if (action == MessageCoder::OK)
        {
            Traces() << "\n" << "LOG: Message OK received";

            if (connection_state == CONNECTED)
            {
                reconnectionTimer.Stop();

                 if (prevousMessageid == data.at(MessageCoder::MESSAGE_ID))
                 {
                     connection_state = ConState::REGISTERED;                                          

                     SendStateMessage(socket, dest, prevousMessageid);
                     reconnectionTimer.Start();
                 } else
                 {
                    Traces() << "\n" << "ERR: Wrong message ID!";

                    SendRegisterMessage(socket, dest, prevousMessageid);
                    reconnectionTimer.Start();
                 }
            } else                
            if (connection_state == REGISTERED)
            {
                reconnectionTimer.Stop();

                 if (prevousMessageid == data.at(MessageCoder::MESSAGE_ID))
                 {
                     connection_state = ConState::STATEUPDATED;
                 } else
                 {
                    Traces() << "\n" << "ERR: Wrong message ID!";

                    SendStateMessage(socket, dest, prevousMessageid);
                    reconnectionTimer.Start();
                 }
            } else
            {
                Traces() << "\n" << "ERR: Wrong connection state";
            }
        } else
        if (action == MessageCoder::CONNECTED)
        {
            Traces() << "\n" << "LOG: action == MessageCoder::CONNECTED";
            connection_state = CONNECTED;
            reconnectionTimer.Start();
            SendRegisterMessage(socket, dest, prevousMessageid);

        } else
        if (action == MessageCoder::CLOSE_CNNECTION)
        {
            Traces() << "\n" << "LOG: action == MessageCoder::CLOSE_CNNECTION";
            connection_state = DISCONNECTED;
            reconnectionTimer.Start();

        } else            
        if (action == MessageCoder::TIMEOUT)
        {
            Traces() << "\n" << "LOG: action == MessageCoder::TIMEOUT";

            connection_state = DISCONNECTED;
            socket.get()->Connect("192.168.0.7", "6000");

        } else
        {
            Traces() << "\n" << "ERR: Unexpected action: " << action;
        }

    }
    catch (std::out_of_range)
    {
        Traces() << "\n" << "ERR: Protocol error";
    }
}

void Worker::SendRegisterMessage(TCPSocket_ptr socket, char * dest, std::string & prevousMessageid)
{
    Traces() << "\n" << "LOG: void Worker::SendRegisterMessage()";

    MessageCoder::ClearChar(dest, MessageCoder::MaxMessageSize());
    prevousMessageid = MessageCoder::CreateMessageId();
    MessageCoder::CreateRoleMessage(MessageCoder::ROLE_ENUM::WORKER, prevousMessageid, dest);
    socket->WriteMessage(dest);
}

void Worker::SendStateMessage(TCPSocket_ptr socket, char * dest, std::string & prevousMessageid)
{
    Traces() << "\n" << "LOG: void Worker::SendStateMessage(TCPSocket_ptr socket, char * dest, std::string & prevousMessageid)";

    MessageCoder::ClearChar(dest, MessageCoder::MaxMessageSize());
    prevousMessageid = MessageCoder::CreateMessageId();

    MessageCoder::CreateStateMessage(myState, maxThread, prevousMessageid, dest);
    socket->WriteMessage(dest);
}

Worker::~Worker()
{
    Traces() << "\n" << "LOG: Worker::~Worker()";
    delete messageQueue;
}
