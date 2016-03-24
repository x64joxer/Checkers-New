#ifndef CHECKERAREA_H
#define CHECKERAREA_H

#include <QDir>
#include <QGraphicsEllipseItem>
#include <QWidget>
#include <QTimer>
#include <QPainter>
#include <QMouseEvent>

#include "Board/Board.h"
#include "Board/PossibleMoves.h"
#include "TCP/TCPHandler.h"
#include "ProgramVariables.h"
#include "ServerState/ServerState.h"


namespace Ui {
class CheckerArea;
}

class CheckerArea : public QWidget
{
        Q_OBJECT

    public:
        explicit CheckerArea(QWidget *parent = 0);
        void SetBoard(Board *wsk);        
        ~CheckerArea();

    public slots:
        void GetServerState(const ServerState &state);

    private:
        Ui::CheckerArea *ui;
        QColor field1;
        QColor field2;
        QColor pawn1;
        QColor pawn2;

        Board *board;
        Board previousBoard;
        unsigned short int displayedBoard;
        ServerState serverState;

        PossibleMoves possibleMoves;

        enum CursorState
        { Free, Grab, WaitForIA } cursorState;

        int mouseX;
        int mouseY;
        unsigned short grabbed;
        unsigned short currentPercentOfSteps;
        unsigned long long startTime;

        QString messageWindow;

        QTimer *waitForIATimer;
        TCPHandler *agentTCP;

        void Paint();
        void PaintFields(QPainter *painter);
        void PaintPawn(QPainter *painter);
        void PaintGrabbedBlackPawn(QPainter *painter);
        void PaintPercentageEllipse(QPainter *painter);
        void PaintMessage(QPainter *painter);
        void DrawPawn(QPainter *painter, const int x, const int y, const int widthField, const int heightField, const bool blackWhite, const bool pons);

        void StartThinking();

        void TakeMouseClickEvent(QMouseEvent *event);
        void TakeMouseReleaseEvent(QMouseEvent *event);
        void TakeMouseMoveEvent(QMouseEvent *event);
        void TakeKeyPressed(QKeyEvent *event);

        void paintEvent(QPaintEvent *);
        void mouseMoveEvent(  QMouseEvent * event);
        void mousePressEvent(QMouseEvent * event);
        void mouseReleaseEvent(QMouseEvent *event);
        void keyPressEvent(QKeyEvent *event);

    private slots:
        void CheckStatus();
        void StateConnecting(const QString data);        
        void StateRegister(const QString server);
        void StateUpdating(const QString server);
        void StateUpdated(const QString server);
};

#endif // CHECKERAREA_H
