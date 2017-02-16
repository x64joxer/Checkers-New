#include "TCPHandler.h"

//██████╗ ██████╗ ███╗   ██╗███████╗████████╗██████╗ ██╗   ██╗ ██████╗████████╗ ██████╗ ██████╗ ███████╗
//██╔════╝██╔═══██╗████╗  ██║██╔════╝╚══██╔══╝██╔══██╗██║   ██║██╔════╝╚══██╔══╝██╔═══██╗██╔══██╗██╔════╝
//██║     ██║   ██║██╔██╗ ██║███████╗   ██║   ██████╔╝██║   ██║██║        ██║   ██║   ██║██████╔╝███████╗
//██║     ██║   ██║██║╚██╗██║╚════██║   ██║   ██╔══██╗██║   ██║██║        ██║   ██║   ██║██╔══██╗╚════██║
//╚██████╗╚██████╔╝██║ ╚████║███████║   ██║   ██║  ██║╚██████╔╝╚██████╗   ██║   ╚██████╔╝██║  ██║███████║
//╚═════╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝   ╚═╝   ╚═╝  ╚═╝ ╚═════╝  ╚═════╝   ╚═╝    ╚═════╝ ╚═╝  ╚═╝╚══════╝

TCPHandler::TCPHandler(QObject *parent) : QObject(parent)
{
    Init();
}

//███████╗██╗   ██╗███╗   ██╗ ██████╗████████╗██╗ ██████╗ ███╗   ██╗███████╗
//██╔════╝██║   ██║████╗  ██║██╔════╝╚══██╔══╝██║██╔═══██╗████╗  ██║██╔════╝
//█████╗  ██║   ██║██╔██╗ ██║██║        ██║   ██║██║   ██║██╔██╗ ██║███████╗
//██╔══╝  ██║   ██║██║╚██╗██║██║        ██║   ██║██║   ██║██║╚██╗██║╚════██║
//██║     ╚██████╔╝██║ ╚████║╚██████╗   ██║   ██║╚██████╔╝██║ ╚████║███████║
//╚═╝      ╚═════╝ ╚═╝  ╚═══╝ ╚═════╝   ╚═╝   ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝

void TCPHandler::ConnectToServer(const QString ho,  int po)
{
    host = ho;
    port = po;

    TRACE_FLAG_FOR_CLASS_TCPHandler Traces() << "\n" << "LOG: Connecting to host:" << host.toStdString() << " port:" << port;

    emit StateConnecting(host + ":" + QString::number(port));    

    if (tcpSocket->isOpen())
    {
        tcpSocket->close();
    } else
    {
        tcpSocket->connectToHost(host,port);
    }

}

void TCPHandler::ConnectionError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        Traces() << "\n" << "ERROR: Disconnected" ;
        connection_state = DISCONNECTED;
        emit StateConnecting(host + ":" + QString::number(port));
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

void TCPHandler::Init()
{
    globalData = new char[MessageCoder::MaxMessageSize()];
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
}

void TCPHandler::SendRegisterMessage()
{
    TRACE_FLAG_FOR_CLASS_TCPHandler Traces() << "\n" << "LOG: void TCPHandler::SendRegisterMessage()";

    emit StateRegister(host + ":" + QString::number(port));
    while(tcpSocket->waitForBytesWritten()) {}
    MessageCoder::ClearChar(globalData, ProgramVariables::K4);
    prevousMessageid = MessageCoder::CreateMessageId();
    MessageCoder::CreateRoleMessage(MessageCoder::ROLE_ENUM::CLIENT, prevousMessageid, globalData);
    tcpSocket->write(globalData);
    while(tcpSocket->waitForBytesWritten()) {}   
}

void TCPHandler::SendGetServerStateMessage()
{
    TRACE_FLAG_FOR_CLASS_TCPHandler Traces() << "\n" << "LOG: void TCPHandler::SendGetServerStateMessage()";

    emit StateUpdating(host + ":" + QString::number(port));
    while(tcpSocket->waitForBytesWritten()) {}
    MessageCoder::ClearChar(globalData, ProgramVariables::K4);
    prevousMessageid = MessageCoder::CreateMessageId();
    MessageCoder::CreateGetServerStateMessage(prevousMessageid, globalData);
    tcpSocket->write(globalData);
    while(tcpSocket->waitForBytesWritten()) {}
}

void TCPHandler::SendOkMessage(const std::string messageId)
{
    TRACE_FLAG_FOR_CLASS_TCPHandler Traces() << "\n" << "LOG: void TCPHandler::SendOkMessage(const std::string messageId)";

    while(tcpSocket->waitForBytesWritten()) {}
    MessageCoder::ClearChar(globalData, ProgramVariables::K4);

    MessageCoder::CreateOkMessage(messageId, globalData);
    tcpSocket->write(globalData);
    while(tcpSocket->waitForBytesWritten()) {}
}

void TCPHandler::SendJob(const Board &board)
{
    TRACE_FLAG_FOR_CLASS_TCPHandler Traces() << "\n" << "LOG: void TCPHandler::SendJob(const Board &board)";

    while(tcpSocket->waitForBytesWritten()) {}

    MessageCoder::ClearChar(globalData, ProgramVariables::K4);
    std::string tempId = MessageCoder::CreateMessageId();
    std::string jobId = MessageCoder::CreateMessageId();
    MessageCoder::CreateStartMessage(ProgramVariables::GetMaxMilisecondsForIa(), 1, tempId, jobId, board, globalData);

    TRACE_FLAG_FOR_CLASS_TCPHandler Traces() << "\n" << "LOG: Sending job to server " << globalData;

    tcpSocket->write(globalData);

    connection_state = WAIT_FOR_STATE_UPDATE;
    waitForOKMessageTimer->start();
}

void TCPHandler::ResetServerState(const Board &board)
{
    TRACE_FLAG_FOR_CLASS_TCPHandler Traces() << "\n" << "LOG: void TCPHandler::ResetServerState(const Board &board)";

    while(tcpSocket->waitForBytesWritten()) {}

    MessageCoder::ClearChar(globalData, ProgramVariables::K4);
    std::string tempId = MessageCoder::CreateMessageId();
    MessageCoder::CreateResetServerStateMessage(board, tempId, globalData);

    TRACE_FLAG_FOR_CLASS_TCPHandler Traces() << "\n" << "LOG: Sending job to server " << globalData;

    tcpSocket->write(globalData);

    connection_state = WAIT_FOR_STATE_UPDATE;
    waitForOKMessageTimer->start();
}

void TCPHandler::Start()
{
    TRACE_FLAG_FOR_CLASS_TCPHandler Traces() << "\n" << "LOG: void TCPHandler::Start()";
    ConnectToServer(ProgramVariables::GetServerIP(), ProgramVariables::GetServerPort());
}

void TCPHandler::DecodeMessage(const char * data)
{
    TRACE_FLAG_FOR_CLASS_TCPHandler Traces() << "\n" << "LOG: void TCPHandler::DecodeMessage(const char * data)";

    std::map<std::string, std::string> messageContent;

    MessageCoder::MessageToMap(data, messageContent);

    try
    {
        std::string action = messageContent.at(MessageCoder::ACTION);

        if (action == MessageCoder::OK)
        {
            TRACE_FLAG_FOR_CLASS_TCPHandler Traces() << "\n" << "LOG: Message OK received";

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
            TRACE_FLAG_FOR_CLASS_TCPHandler Traces() << "\n" << "LOG: Message SERVER_STATE received";

            if (connection_state == REGISTERED)
            {
                waitForOKMessageTimer->stop();

                if (prevousMessageid == messageContent.at(MessageCoder::MESSAGE_ID))
                {

                    Board tmpBoard;
                    MessageCoder::MapToBoard(messageContent, &tmpBoard);

                    emit ServerStateReceived(ServerState(tmpBoard,
                                                         atof(messageContent.at((MessageCoder::IS_THINKING)).c_str()),
                                                         atoll(messageContent.at((MessageCoder::START_TIME)).c_str()),
                                                         atoll(messageContent.at((MessageCoder::MAX_IA_TIME)).c_str()),
                                                         atoll(messageContent.at((MessageCoder::TIME_TO_END)).c_str()),
                                                         messageContent.at((MessageCoder::SERVER_ERROR)),
                                                         std::stoi(messageContent.at((MessageCoder::WHITE_WINS)).c_str()),
                                                         std::stoi(messageContent.at((MessageCoder::BLACK_WINS)).c_str())
                                                         ));

                    connection_state = ConState::UPDATED;

                    emit StateUpdated(host + ":" + QString::number(port));
                } else
                {
                   Traces() << "\n" << "ERR: Wrong message ID!";

                   SendGetServerStateMessage();
                   waitForOKMessageTimer->start();
                }
            }
            else
            if (connection_state == UPDATED)
            {
                Board tmpBoard;
                MessageCoder::MapToBoard(messageContent, &tmpBoard);

                emit ServerStateReceived(ServerState(tmpBoard,
                                                     atof(messageContent.at((MessageCoder::IS_THINKING)).c_str()),
                                                     atoll(messageContent.at((MessageCoder::START_TIME)).c_str()),
                                                     atoll(messageContent.at((MessageCoder::MAX_IA_TIME)).c_str()),
                                                     atoll(messageContent.at((MessageCoder::TIME_TO_END)).c_str()),
                                                     messageContent.at((MessageCoder::SERVER_ERROR)),
                                                     std::stoi(messageContent.at((MessageCoder::WHITE_WINS)).c_str()),
                                                     std::stoi(messageContent.at((MessageCoder::BLACK_WINS)).c_str())
                                                     ));

               SendOkMessage(messageContent.at(MessageCoder::MESSAGE_ID));
            } else
            if (connection_state == WAIT_FOR_STATE_UPDATE)
            {
                TRACE_FLAG_FOR_CLASS_TCPHandler Traces() << "\n" << "LOG: connection_state == WAIT_FOR_STATE_UPDATE";

                waitForOKMessageTimer->stop();
                Board tmpBoard;
                MessageCoder::MapToBoard(messageContent, &tmpBoard);

                emit ServerStateReceived(ServerState(tmpBoard,
                                                     atof(messageContent.at((MessageCoder::IS_THINKING)).c_str()),
                                                     atoll(messageContent.at((MessageCoder::START_TIME)).c_str()),
                                                     atoll(messageContent.at((MessageCoder::MAX_IA_TIME)).c_str()),
                                                     atoll(messageContent.at((MessageCoder::TIME_TO_END)).c_str()),
                                                     messageContent.at((MessageCoder::SERVER_ERROR)),
                                                     std::stoi(messageContent.at((MessageCoder::WHITE_WINS)).c_str()),
                                                     std::stoi(messageContent.at((MessageCoder::BLACK_WINS)).c_str())
                                                     ));


               SendOkMessage(messageContent.at(MessageCoder::MESSAGE_ID));

               connection_state = UPDATED;
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

//███████╗██╗      ██████╗ ████████╗███████╗
//██╔════╝██║     ██╔═══██╗╚══██╔══╝██╔════╝
//███████╗██║     ██║   ██║   ██║   ███████╗
//╚════██║██║     ██║   ██║   ██║   ╚════██║
//███████║███████╗╚██████╔╝   ██║   ███████║
//╚══════╝╚══════╝ ╚═════╝    ╚═╝   ╚══════╝

void TCPHandler::Connected()
{
    TRACE_FLAG_FOR_CLASS_TCPHandler Traces() << "\n" << "LOG: SUCCES! Connected to host:"  << host.toStdString() << " port:" << port;;

    time->start();
    connection_state = CONNECTED;
}

void TCPHandler::Disconnect()
{
    TRACE_FLAG_FOR_CLASS_TCPHandler Traces() << "\n" << "LOG: void TCPHandler::Disconnect()";

    connection_state = DISCONNECTED;
    tcpSocket->close();
    time->start();
}

void TCPHandler::NoResponseFromServer()
{
    TRACE_FLAG_FOR_CLASS_TCPHandler Traces() << "\n" << "LOG: void TCPHandler::NoResponseFromServer()";

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
    if (connection_state == WAIT_FOR_STATE_UPDATE)
    {
        Traces() << "\n" << "ERR: connection_state == WAIT_FOR_STATE_UPDATE emit FailedSendJob()";

        connection_state = UPDATED;
        emit FailedSendJob("");

    } else
    if (connection_state == DISCONNECTED)
    {

    };
}

void TCPHandler::Reconnect()
{
    time->stop();

    if (connection_state == ConState::DISCONNECTED)
    {
        TRACE_FLAG_FOR_CLASS_TCPHandler Traces() << "\n" << "LOG: Reconnecting to host:"  << host.toStdString() << " port:" << port;;
        emit StateConnecting(host + ":" + QString::number(port));
        tcpSocket->connectToHost(host,port);
    } else
    if (connection_state == ConState::CONNECTED)
    {
        TRACE_FLAG_FOR_CLASS_TCPHandler Traces() << "\n" << "LOG: Registering as client";
        SendRegisterMessage();
        waitForOKMessageTimer->start();

    } else
    {
        Traces() << "\n" << "ERROR: Wrong connection state!";
    }
}

void TCPHandler::ReadDataFromServer()
{
    MessageCoder::ClearChar(globalData, ProgramVariables::K4);
    strcpy(globalData, tcpSocket->readAll().toStdString().c_str());

    TRACE_FLAG_FOR_CLASS_TCPHandler Traces() << "\n" << "LOG: Read data from server: " <<  std::string(globalData);

    DecodeMessage(globalData);

}


//██████╗ ███████╗███████╗████████╗██████╗ ██╗   ██╗ ██████╗████████╗ ██████╗ ██████╗
//██╔══██╗██╔════╝██╔════╝╚══██╔══╝██╔══██╗██║   ██║██╔════╝╚══██╔══╝██╔═══██╗██╔══██╗
//██║  ██║█████╗  ███████╗   ██║   ██████╔╝██║   ██║██║        ██║   ██║   ██║██████╔╝
//██║  ██║██╔══╝  ╚════██║   ██║   ██╔══██╗██║   ██║██║        ██║   ██║   ██║██╔══██╗
//██████╔╝███████╗███████║   ██║   ██║  ██║╚██████╔╝╚██████╗   ██║   ╚██████╔╝██║  ██║
//╚═════╝ ╚══════╝╚══════╝   ╚═╝   ╚═╝  ╚═╝ ╚═════╝  ╚═════╝   ╚═╝    ╚═════╝ ╚═╝  ╚═╝

TCPHandler::~TCPHandler()
{
    delete globalData;
    delete waitForOKMessageTimer;
    delete waitForIATimer;
    delete tcpSocket;
    delete time;
}
