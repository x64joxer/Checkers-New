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
    Traces() << "\n" << "LOG: Reconnecting to host:"  << host.toStdString() << " port:" << port;;
    tcpSocket->connectToHost(host,port);
}

void TCPHandler::SendJob(const Board &board)
{

}

void TCPHandler::Connected()
{
    Traces() << "\n" << "LOG: SUCCES! Connected to host:"  << host.toStdString() << " port:" << port;;

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


void TCPHandler::ReadDataFromServer()
{
    Traces() << "\n" << "LOG: Read data from server";
}

void TCPHandler::Disconnect()
{
    tcpSocket->disconnectFromHost();
    time->start();
}

void TCPHandler::NoResponseFromServer()
{

}

TCPHandler::~TCPHandler()
{
    delete globalData;
    delete waitForOKMessageTimer;
    delete waitForIATimer;
    delete tcpSocket;
    delete time;
}
