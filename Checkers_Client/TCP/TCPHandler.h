#ifndef TCPHANDLER_H
#define TCPHANDLER_H

#include <QTcpSocket>
#include <QTcpServer>
#include <QTimer>
#include "Board/Board.h"
#include "MessageCoder/MessageCoder.h"
#include "ProgramVariables.h"

class TCPHandler : public QObject
{
        Q_OBJECT
    public:
        explicit TCPHandler(QObject *parent = 0);

        void ConnectToServer(const QString ho, int po);
        void SendJob(const Board &board);

        enum MessageState { NONE_OK, STATE_OK, BEST_RESULT_OK };

        ~TCPHandler();
    signals:

    public slots:
        void ReadDataFromServer();
        void Reconnect();
        void ConnectionError(QAbstractSocket::SocketError socketError);
        void Connected();

    private slots:
        void NoResponseFromServer();

    private:
        void Init();
        void Disconnect();

        QTcpSocket *tcpSocket;
        enum ConState { DISCONNECTED, CONNECTED} connection_state;
        MessageState messageState;
        unsigned short numOfReattempt;
        QTimer *time;
        QString host;
        int port;

        char *globalData;
        unsigned int globalLength;

        QTimer *waitForIATimer;
        QTimer *waitForOKMessageTimer;

};

#endif // TCPHANDLER_H
