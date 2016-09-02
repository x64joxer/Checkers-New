#include "CheckerArea.h"
#include "ui_CheckerArea.h"

//██████╗ ██████╗ ███╗   ██╗███████╗████████╗██████╗ ██╗   ██╗ ██████╗████████╗ ██████╗ ██████╗ ███████╗
//██╔════╝██╔═══██╗████╗  ██║██╔════╝╚══██╔══╝██╔══██╗██║   ██║██╔════╝╚══██╔══╝██╔═══██╗██╔══██╗██╔════╝
//██║     ██║   ██║██╔██╗ ██║███████╗   ██║   ██████╔╝██║   ██║██║        ██║   ██║   ██║██████╔╝███████╗
//██║     ██║   ██║██║╚██╗██║╚════██║   ██║   ██╔══██╗██║   ██║██║        ██║   ██║   ██║██╔══██╗╚════██║
//╚██████╗╚██████╔╝██║ ╚████║███████║   ██║   ██║  ██║╚██████╔╝╚██████╗   ██║   ╚██████╔╝██║  ██║███████║
//╚═════╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝   ╚═╝   ╚═╝  ╚═╝ ╚═════╝  ╚═════╝   ╚═╝    ╚═════╝ ╚═╝  ╚═╝╚══════╝

CheckerArea::CheckerArea(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CheckerArea)
{
    ui->setupUi(this);

    setFocusPolicy(Qt::TabFocus);

    board = nullptr;

    field1 = QColor(0,0,0);
    field2 = QColor(255,255,255);

    pawn1 = QColor(0,0,255);
    pawn2 = QColor(255,0,0);    

    cursorState = Free;

    disconnectTimer = new QTimer();    
    disconnectTimer->setInterval(10);

    clearMessageTimer = new QTimer();
    connect(clearMessageTimer,SIGNAL(timeout()), this, SLOT(ClearStateMessage()));

    displayedBoard = 0;
    waitForIATimer = new QTimer();
    waitForIATimer->setInterval(50);
    connect(waitForIATimer,SIGNAL(timeout()), this, SLOT(CheckStatus()));

    agentTCP = new TCPHandler(this);

    connect(agentTCP, SIGNAL(ServerStateReceived(ServerState)), this, SLOT(GetServerState(ServerState)));
    connect(agentTCP, SIGNAL(StateConnecting(const QString)), this, SLOT(StateConnecting(const QString)));
    connect(agentTCP, SIGNAL(StateRegister(const QString)), this, SLOT(StateRegister(const QString)));
    connect(agentTCP, SIGNAL(StateUpdating(const QString)), this, SLOT(StateUpdating(const QString)));
    connect(agentTCP, SIGNAL(StateUpdated(QString)), this, SLOT(StateUpdated(const QString)));
    connect(agentTCP, SIGNAL(FailedSendJob(const QString)), this, SLOT(ProblemWithSendingJob(QString)));

    connect(disconnectTimer, SIGNAL(timeout()), this, SLOT(DisconnectFromServer()));
    connect(this, SIGNAL(Disconnect()), agentTCP, SLOT(Disconnect()));

    agentTCP->Start();

}

//███████╗██╗   ██╗███╗   ██╗ ██████╗████████╗██╗ ██████╗ ███╗   ██╗███████╗
//██╔════╝██║   ██║████╗  ██║██╔════╝╚══██╔══╝██║██╔═══██╗████╗  ██║██╔════╝
//█████╗  ██║   ██║██╔██╗ ██║██║        ██║   ██║██║   ██║██╔██╗ ██║███████╗
//██╔══╝  ██║   ██║██║╚██╗██║██║        ██║   ██║██║   ██║██║╚██╗██║╚════██║
//██║     ╚██████╔╝██║ ╚████║╚██████╗   ██║   ██║╚██████╔╝██║ ╚████║███████║
//╚═╝      ╚═════╝ ╚═╝  ╚═══╝ ╚═════╝   ╚═╝   ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝

void CheckerArea::Paint()
{    
    QPainter painter;
    painter.begin(this);

    PaintFields(&painter);
    PaintPawn(&painter);
    PaintGrabbedBlackPawn(&painter);
    PaintPercentageEllipse(&painter);    
    PaintMessage(&painter);

    painter.end();
}

void CheckerArea::SetBoard(Board *wsk)
{
    board = wsk;
}

void CheckerArea::SetMessageText(const QString & text)
{
    messageWindow = text;
}

void CheckerArea::PaintFields(QPainter *painter)
{
    int widthField = width() / 8;
    int heightField = height() / 8;
    bool flag = 0;

    painter->setPen(QColor(0,0,0));

    for (int y = 0;y<8;y++)
    {
        for (int x = 0;x<8;x++)
        {
            if (flag)
            {
                painter->setBrush(field1);
            } else
            {
                painter->setBrush(field2);
            };

            flag = !flag;


            painter->drawRect(x*widthField,
                              y*heightField,
                              (x*widthField)+widthField,
                              (y*heightField)+heightField);            

        };
        flag = !flag;
    };
}

void CheckerArea::PaintPawn(QPainter *painter)
{
    Board *boardToPaint;

    if (cursorState == WaitForIA)
    {
        boardToPaint = &previousBoard;
    } else
    {
        if (displayedBoard == 0)
        {
          boardToPaint = board;
        } else
        {
          boardToPaint = &previousBoard;
        };
    };

    if (boardToPaint == nullptr) Traces() << "\n" << "ERROR! CheckerArea::PaintPawn(QPainter *painter) RPointer to board is empty!";
    ushort numberOfWhite = boardToPaint->GetNumberOfWhite();
    ushort numberOfBlack = boardToPaint->GetNumberOfBlack();
    int widthField = width() / 8;
    int heightField = height() / 8;
    PawnPos pos;

    for (int i=0;i<numberOfWhite;i++)
    {
        pos = boardToPaint->GetWhitePawnPos(i);

        DrawPawn(painter,
                 (pos.X() * widthField) + (widthField /2),
                 (pos.Y() * heightField) + (heightField /2),
                 widthField,
                 heightField,1,
                 boardToPaint->GetWhitePawnPons(i));

    };

    for (int i=0;i<numberOfBlack;i++)
    {
        pos = boardToPaint->GetBlackPawnPos(i);

        DrawPawn(painter,
                 (pos.X() * widthField) + (widthField /2),
                 (pos.Y() * heightField) + (heightField /2),
                 widthField,
                 heightField,
                 0,
                 boardToPaint->GetBlackPawnPons(i));

    };
}

void CheckerArea::PaintGrabbedBlackPawn(QPainter *painter)
{
    if (cursorState == Grab)
    {
        int widthField = width() / 8;
        int heightField = height() / 8;

        DrawPawn(painter, mouseX, mouseY, widthField, heightField, 0, board->GetBlackPawnPons(grabbed));
    };
}

void CheckerArea::PaintPercentageEllipse(QPainter *painter)
{

    if (cursorState == WaitForIA)
    {
        currentPercentOfSteps = (Traces::GetCurrentDateLL() / 1000) - (startTime / 1000);
        currentPercentOfSteps = ((serverState.GetMaxTime() - currentPercentOfSteps) * 100) / serverState.GetMaxTime();

        currentPercentOfSteps = 100 - currentPercentOfSteps;

        if (currentPercentOfSteps == 100)
        {
            SetMessageText("Waiting for result from server...");
            waitForIATimer->stop();
            disconnectTimer->start(ProgramVariables::GetMaxTimeWaitToServer());
        }

        int size = 5;
        QRectF rectangle(width() -(width() / size) -10,
                         height() - (height() / size) - 10,
                         (width() / size),
                         (height() / size));

        int startAngle = 0 * 16;
        int spanAngle;

        if (currentPercentOfSteps == 0)
        {
            spanAngle = ((((360*16)) * currentPercentOfSteps) /100);
        } else
        {
            spanAngle = (((360*16)) * (currentPercentOfSteps+1) /100);
        };

        if (currentPercentOfSteps == 99) spanAngle = 360*16;

        painter->setBrush(QColor(0,255,0));
        painter->setPen(QColor(0,255,0));

        painter->drawPie(rectangle,
                         startAngle,
                         spanAngle);
    };
}

void CheckerArea::PaintMessage(QPainter *painter)
{  
    if (messageWindow != "")
    {
        QRectF rectangle = QRectF(0,
                  (height() / 2) - ((height() / 7) / 2),
                  width(),
                  (height() / 7));

        painter->setBrush(QColor(0,255,255));
        painter->setPen(QColor(0,255,255));
        painter->drawRect(rectangle);

        painter->setBrush(QColor(0, 0, 0));
        painter->setPen(QColor(0, 0, 0));

        QFont font;
        font.setPixelSize(rectangle.height() / 1.5);
        painter->setFont(font);



        painter->drawText(rectangle.x(),
                          rectangle.y(),
                          rectangle.width(),
                          rectangle.height(),
                          Qt::AlignCenter,
                          messageWindow);
    }
}

void CheckerArea::DrawPawn(QPainter *painter, const int x, const int y, const int widthField, const int heightField, const bool blackWhite, const bool pons)
{
    if (blackWhite)
    {
        painter->setBrush(pawn1);
    } else
    {
        painter->setBrush(pawn2);
    };

    if (!pons)
    {

        painter->drawEllipse(QPoint(x,y),
                             widthField / 2,
                             heightField / 2);

        painter->drawEllipse(QPoint(x,y),
                             widthField / 2.7,
                             heightField / 2.7);
    } else
    {
        painter->drawEllipse(QPoint(x,y),
                             widthField / 2,
                             heightField / 2);

        painter->drawEllipse(QPoint(x,y),
                             widthField / 2.3,
                             heightField / 2.3);
    };
}

void CheckerArea::StartThinking()
{
    agentTCP->SendJob(*board);
}

void CheckerArea::TakeMouseClickEvent(QMouseEvent *event)
{
    int widthField = width() / 8;
    int heightField = height() / 8;

    unsigned short x = (event->x() / widthField);
    unsigned short y = (event->y() / heightField);

    if (cursorState == Free)
    {
        if (agentTCP->GecConnectionState() == TCPHandler::ConState::UPDATED)
        {
            if (board->IsBlackPawnOnPos(x,y))
            {
                grabbed = board->GetBlackPawnNumber(x,y);

                if (possibleMoves.CanIGrab(grabbed, *board))
                {
                    cursorState = Grab;
                };
            };
        }
    };
}

void CheckerArea::TakeMouseReleaseEvent(QMouseEvent *event)
{    
    int widthField = width() / 8;
    int heightField = height() / 8;

    unsigned short x = (event->x() / widthField);
    unsigned short y = (event->y() / heightField);

    if (cursorState == Grab)
    {                
        Traces() << "\n" << "if (cursorState == Grab)";
        previousBoard = *board;

        if (possibleMoves.CanIPutHere(grabbed, x, y, *board))
        {
            unsigned short killed;
            if (possibleMoves.IsKill(grabbed, x, y, *board, killed))
            {
                board->RemoveWhitePawn(killed);

                board->SetBlackPawnPos(grabbed,x,y);
                if (possibleMoves.CheckHitTopLeft(x,y,*board) | possibleMoves.CheckHitTopRight(x,y,*board))
                {
                    Traces() << "\n" << "After kill possibleMoves.CheckHitTopLeft(x,y,*board) | possibleMoves.CheckHitTopRight(x,y,*board) is true, cursorState = Free";
                    cursorState = Free;
                } else
                if (board->GetBlackPawnPons(grabbed))
                {
                    if (possibleMoves.CheckHitBottomLeft(x,y,*board) | possibleMoves.CheckHitBottomRight(x,y,*board))
                    {
                        Traces() << "\n" << "cursorState = Free";
                        cursorState = Free;
                    } //Error here
                    else
                    {
                        if (board->GetNumberOfWhite()>0)
                        {
                            Traces() << "\n" << "cursorState = WaitForSerwerStateUpdate";                            
                            cursorState = WaitForSerwerStateUpdate;
                            SendingJob("");
                            StartThinking();                            
                        } else cursorState = Free;
                    };
                }
                else
                {
                    if (board->GetNumberOfWhite()>0)
                    {
                        Traces() << "\n" << "cursorState = WaitForSerwerStateUpdate";                        
                        cursorState = WaitForSerwerStateUpdate;
                        SendingJob("");
                        StartThinking();                        
                    } else cursorState = Free;
                };
            } else
            {
                if (board->GetNumberOfWhite()>0)
                {
                    Traces() << "\n" << "cursorState = WaitForSerwerStateUpdate";
                    board->SetBlackPawnPos(grabbed,x,y);                    
                    cursorState = WaitForSerwerStateUpdate;
                    SendingJob("");
                    StartThinking();
                } else cursorState = Free;
            }
        } else
        {
            cursorState = Free;
            Traces() << "\n" << "cursorState = Free";
        };
    };    
    repaint();
}

void CheckerArea::TakeMouseMoveEvent(QMouseEvent *event)
{
    mouseX = event->x();
    mouseY = event->y();

    if (cursorState == Grab) repaint();

}

void CheckerArea::TakeKeyPressed(QKeyEvent *event)
{
    if (event->key()==Qt::Key_Up)
    {
       displayedBoard = 1;
       repaint();
    } else
    if (event->key()==Qt::Key_Down)
    {
       displayedBoard = 0;
       repaint();
    };
}

//███████╗██╗      ██████╗ ████████╗███████╗
//██╔════╝██║     ██╔═══██╗╚══██╔══╝██╔════╝
//███████╗██║     ██║   ██║   ██║   ███████╗
//╚════██║██║     ██║   ██║   ██║   ╚════██║
//███████║███████╗╚██████╔╝   ██║   ███████║
//╚══════╝╚══════╝ ╚═════╝    ╚═╝   ╚══════╝

void CheckerArea::CheckStatus()
{
    repaint();
}

void CheckerArea::DisconnectFromServer()
{
    Traces() << "\n" << "LOG: void CheckerArea::DisconnectFromServer()";

    disconnectTimer->stop();
    cursorState = Free;
    SetMessageText("");
    emit Disconnect();
}

void CheckerArea::GetServerState(const ServerState &state)
{
    Traces() << "\n" << "LOG: void CheckerArea::GetServerState(const ServerState &state)";

    serverState = state;
    *board = serverState.GetBoard();

    if (serverState.IsThinking())
    {
        cursorState = WaitForIA;
        startTime = Traces::GetCurrentDateLL();
        waitForIATimer->start();
        clearMessageTimer->stop();
        ClearStateMessage();
    }

    repaint();
}

void CheckerArea::ClearStateMessage()
{
    Traces() << "\n" << "LOG: void CheckerArea::ClearStateMessage()";
    messageWindow = "";
    clearMessageTimer->stop();
    repaint();
}

void CheckerArea::StateConnecting(const QString data)
{
    Traces() << "\n" << "LOG: void CheckerArea::StateConnecting(const QString data)";

    clearMessageTimer->stop();
    messageWindow = "Connecting to: " + data;
    repaint();
}

void CheckerArea::StateRegister(const QString server)
{
    Traces() << "\n" << "LOG: void CheckerArea::StateRegister(const QString server)";

    clearMessageTimer->stop();
    messageWindow = "Registering...";
    repaint();
}

void CheckerArea::StateUpdating(const QString server)
{
    Traces() << "\n" << "LOG: void CheckerArea::StateUpdating(const QString server)";

    clearMessageTimer->stop();
    messageWindow = "Updating status...";
    repaint();
}

void CheckerArea::StateUpdated(const QString server)
{
    Traces() << "\n" << "LOG: void CheckerArea::StateUpdated(const QString server)";
    messageWindow = "";
    repaint();
}

void CheckerArea::SendingJob(const QString server)
{
    Traces() << "\n" << "LOG: void CheckerArea::SendingJob(const QString server)";

    clearMessageTimer->stop();
    messageWindow = "Sending job...";
    repaint();
}

void CheckerArea::ProblemWithSendingJob(const QString server)
{
    Traces() << "\n" << "LOG: void CheckerArea::ProblemWithSendingJob(const QString server)";

    clearMessageTimer->stop();
    messageWindow = "Problem with sending job to server";
    *board = previousBoard;
    cursorState = CursorState::Free;
    clearMessageTimer->start(5000);
    repaint();
}

//███████╗██╗   ██╗███████╗███╗   ██╗████████╗███████╗
//██╔════╝██║   ██║██╔════╝████╗  ██║╚══██╔══╝██╔════╝
//█████╗  ██║   ██║█████╗  ██╔██╗ ██║   ██║   ███████╗
//██╔══╝  ╚██╗ ██╔╝██╔══╝  ██║╚██╗██║   ██║   ╚════██║
//███████╗ ╚████╔╝ ███████╗██║ ╚████║   ██║   ███████║
//╚══════╝  ╚═══╝  ╚══════╝╚═╝  ╚═══╝   ╚═╝   ╚══════╝

void CheckerArea::paintEvent(QPaintEvent *)
{
    Paint();
}

void CheckerArea::mouseMoveEvent(  QMouseEvent * event)
{
    TakeMouseMoveEvent(event);
}

void CheckerArea::mousePressEvent(QMouseEvent * event)
{
    TakeMouseClickEvent(event);
}

void CheckerArea::mouseReleaseEvent(QMouseEvent *event)
{
    TakeMouseReleaseEvent(event);
}

void CheckerArea::keyPressEvent(QKeyEvent *event)
{
    TakeKeyPressed(event);
}

//██████╗ ███████╗███████╗████████╗██████╗ ██╗   ██╗ ██████╗████████╗ ██████╗ ██████╗
//██╔══██╗██╔════╝██╔════╝╚══██╔══╝██╔══██╗██║   ██║██╔════╝╚══██╔══╝██╔═══██╗██╔══██╗
//██║  ██║█████╗  ███████╗   ██║   ██████╔╝██║   ██║██║        ██║   ██║   ██║██████╔╝
//██║  ██║██╔══╝  ╚════██║   ██║   ██╔══██╗██║   ██║██║        ██║   ██║   ██║██╔══██╗
//██████╔╝███████╗███████║   ██║   ██║  ██║╚██████╔╝╚██████╗   ██║   ╚██████╔╝██║  ██║
//╚═════╝ ╚══════╝╚══════╝   ╚═╝   ╚═╝  ╚═╝ ╚═════╝  ╚═════╝   ╚═╝    ╚═════╝ ╚═╝  ╚═╝

CheckerArea::~CheckerArea()
{    
    delete agentTCP;
    delete disconnectTimer;
    delete clearMessageTimer;
    delete ui;
}
