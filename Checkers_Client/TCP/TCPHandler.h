#ifndef TCPHANDLER_H
#define TCPHANDLER_H

#include <QTcpSocket>
#include <QTcpServer>
#include <QTimer>
#include "Board/Board.h"
#include "MessageCoder/MessageCoder.h"
#include "../ProgramVariables/ProgramVariables.h"
#include "ServerState/ServerState.h"

class TCPHandler : public QObject
{
        Q_OBJECT
    public:
        explicit TCPHandler(QObject *parent = 0);

        void ConnectToServer(const QString ho, int po);
        void SendJob(const Board &board);
        void ResetServerState(const Board &board);
        void Start();

        enum MessageState { NONE_OK, STATE_OK, BEST_RESULT_OK };
        enum ConState { DISCONNECTED, CONNECTED, REGISTERED, UPDATED, WAIT_FOR_STATE_UPDATE };
        ConState GecConnectionState() { return connection_state; }

        ~TCPHandler();
    signals:
        void ServerStateReceived(const ServerState state);
        void StateConnecting(const QString server);
        void StateRegister(const QString server);
        void StateUpdating(const QString server);
        void StateUpdated(const QString server);
        void FailedSendJob(const QString server);


    public slots:
        void Disconnect();

    private slots:
        void NoResponseFromServer();
        void ReadDataFromServer();
        void Reconnect();
        void ConnectionError(QAbstractSocket::SocketError socketError);
        void Connected();

    private:
        void Init();       
        void DecodeMessage(const char * data);
        void SendRegisterMessage();
        void SendGetServerStateMessage();
        void SendOkMessage(const std::string messageId);

        QTcpSocket *tcpSocket;
        ConState connection_state;
        MessageState messageState;
        unsigned short numOfReattempt;
        QTimer *time;
        QString host;
        int port;
        std::string prevousMessageid;

        char *globalData;
        unsigned int globalLength;        

        QTimer *waitForIATimer;
        QTimer *waitForOKMessageTimer;

};

#endif // TCPHANDLER_H
