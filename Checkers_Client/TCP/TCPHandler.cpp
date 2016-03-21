#include "TCPHandler.h"

TCPHandler::TCPHandler(QObject *parent) : QObject(parent)
{
    Init();
}


void TCPHandler::Init()
{       
    globalData = new char[ProgramVariables::K4];
    globalLength = 0;

    time = new QTimer();
    connect(time,SIGNAL(timeout()),this,SLOT(Reconnect()));

    waitForOKMessageTimer = new QTimer();
    connect(waitForOKMessageTimer,SIGNAL(timeout()),this,SLOT(NoResponseFromServer()));
    waitForOKMessageTimer->setInterval(ProgramVariables::GetMaxTimeWaitToServer());

    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(ReadDataFromServer()));
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(ConnectionError(QAbstractSocket::SocketError)));
    connect(tcpSocket,SIGNAL(connected()),this,SLOT(Connected()));

    waitForIATimer = new QTimer();
    waitForIATimer->setInterval(10);

    ConnectToServer(ProgramVariables::GetServerIP(), ProgramVariables::GetServerPort());
}

void TCPHandler::ConnectToServer(const QString ho,  int po)
{
    host = ho;
    port = po;

    Traces() << "\n" << "LOG: Connecting to host:" << host.toStdString() << " port:" << port;
    tcpSocket->connectToHost(host,port);
}

void TCPHandler::Reconnect()
{
    time->stop();

    if (connection_state == ConState::DISCONNECTED)
    {
        Traces() << "\n" << "LOG: Reconnecting to host:"  << host.toStdString() << " port:" << port;;
        tcpSocket->connectToHost(host,port);
    } else
    if (connection_state == ConState::CONNECTED)
    {
        Traces() << "\n" << "LOG: Registering as client";
        SendRegisterMessage();
        waitForOKMessageTimer->start();

    } else
    {
        Traces() << "\n" << "ERROR: Wrong connection state!";
    }
}

void TCPHandler::SendRegisterMessage()
{
    Traces() << "\n" << "LOG: void TCPHandler::SendRegisterMessage()";

    while(tcpSocket->waitForBytesWritten()) {}
    MessageCoder::ClearChar(globalData, ProgramVariables::K4);
    prevousMessageid = MessageCoder::CreateMessageId();
    MessageCoder::CreateRoleMessage(MessageCoder::ROLE_ENUM::CLIENT, prevousMessageid, globalData);
    tcpSocket->write(globalData);
    while(tcpSocket->waitForBytesWritten()) {}   
}

void TCPHandler::SendGetServerStateMessage()
{
    Traces() << "\n" << "LOG: void TCPHandler::SendGetServerStateMessage()";

    while(tcpSocket->waitForBytesWritten()) {}
    MessageCoder::ClearChar(globalData, ProgramVariables::K4);
    prevousMessageid = MessageCoder::CreateMessageId();
    MessageCoder::CreateGetServerStateMessage(prevousMessageid, globalData);
    tcpSocket->write(globalData);
    while(tcpSocket->waitForBytesWritten()) {}
}

void TCPHandler::SendJob(const Board &board)
{
    while(tcpSocket->waitForBytesWritten()) {}

    MessageCoder::ClearChar(globalData, ProgramVariables::K4);
    std::string tempId = MessageCoder::CreateMessageId();
    std::string jobId = MessageCoder::CreateMessageId();
    MessageCoder::CreateStartMessage(ProgramVariables::GetMaxTimeForIa(), 1, tempId, jobId, globalData);
    MessageCoder::BoardToChar(board, globalData, 1);

    Traces() << "\n" << "LOG: Sending job to server " << globalData;

    tcpSocket->write(globalData);
}

void TCPHandler::Connected()
{
    Traces() << "\n" << "LOG: SUCCES! Connected to host:"  << host.toStdString() << " port:" << port;;

    time->start();
    connection_state = CONNECTED;        
}

void TCPHandler::ConnectionError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        Traces() << "\n" << "ERROR:The host was not found. Please check the host name and port settings";
        break;
    case QAbstractSocket::ConnectionRefusedError:
        Traces() << "\n" << "ERROR:The connection was refused by the peer" ;
        connection_state = DISCONNECTED;
        break;
    default:
        Traces() << "\n" << "ERROR:The following error occurred:" << tcpSocket->errorString().toStdString();
        connection_state = DISCONNECTED;
    }

    time->setInterval(ProgramVariables::GetRecconectingTime());
    time->start();
}

void TCPHandler::DecodeMessage(const char * data)
{
    Traces() << "\n" << "LOG: void TCPHandler::DecodeMessage(const char * data)";

    std::map<std::string, std::string> messageContent;

    MessageCoder::MessageToMap(data, messageContent);

    try
    {
        std::string action = messageContent.at(MessageCoder::ACTION);

        if (action == MessageCoder::OK)
        {
            Traces() << "\n" << "LOG: Message OK received";

            if (connection_state == CONNECTED)
            {
                waitForOKMessageTimer->stop();

                 if (prevousMessageid == messageContent.at(MessageCoder::MESSAGE_ID))
                 {
                     connection_state = ConState::REGISTERED;
                     SendGetServerStateMessage();
                     waitForOKMessageTimer->start();
                 } else
                 {
                    Traces() << "\n" << "ERR: Wrong message ID!";

                    SendRegisterMessage();
                    waitForOKMessageTimer->start();
                 }
            } else
            {
                Traces() << "\n" << "ERR: Wrong connection state";
            }
        } else
        if (action == MessageCoder::SERVER_STATE)
        {
            Traces() << "\n" << "LOG: Message SERVER_STATE received";

            if (connection_state == REGISTERED)
            {
                waitForOKMessageTimer->stop();

                if (prevousMessageid == messageContent.at(MessageCoder::MESSAGE_ID))
                {
                    connection_state = ConState::UPDATED;

                } else
                {
                   Traces() << "\n" << "ERR: Wrong message ID!";

                   SendGetServerStateMessage();
                   waitForOKMessageTimer->start();
                }

            } else
            {
                Traces() << "\n" << "ERR: Wrong connection state";
            }

        } else
        {
            Traces() << "\n" << "ERR: Wrong action = " << action;
        }

    }
    catch (std::out_of_range)
    {
        Traces() << "\n" << "ERR: Protocol error host: " << tcpSocket->peerAddress().toString().toStdString() << " " << tcpSocket->peerPort();
    }
}

void TCPHandler::ReadDataFromServer()
{
    MessageCoder::ClearChar(globalData, ProgramVariables::K4);
    strcpy(globalData, tcpSocket->readAll().toStdString().c_str());

    Traces() << "\n" << "LOG: Read data from server: " <<  std::string(globalData);

    DecodeMessage(globalData);

}

void TCPHandler::Disconnect()
{
    tcpSocket->disconnectFromHost();
    time->start();
}

void TCPHandler::NoResponseFromServer()
{
    Traces() << "\n" << "LOG: void TCPHandler::NoResponseFromServer()";

    waitForOKMessageTimer->stop();

    if (connection_state == CONNECTED)
    {
        SendRegisterMessage();
        waitForOKMessageTimer->start();
    } else
    if (connection_state == REGISTERED)
    {
        SendGetServerStateMessage();
        waitForOKMessageTimer->start();
    } else
    if (connection_state == DISCONNECTED)
    {

    };
}

TCPHandler::~TCPHandler()
{
    delete globalData;
    delete waitForOKMessageTimer;
    delete waitForIATimer;
    delete tcpSocket;
    delete time;
}
