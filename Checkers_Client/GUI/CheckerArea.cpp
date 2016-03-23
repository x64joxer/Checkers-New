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

    displayedBoard = 0;
    waitForIATimer = new QTimer();
    waitForIATimer->setInterval(50);
    connect(waitForIATimer,SIGNAL(timeout()), this, SLOT(CheckStatus()));

    agentTCP = new TCPHandler(this);

    connect(agentTCP, SIGNAL(ServerStateReceived(ServerState)), this, SLOT(GetServerState(ServerState)));
    connect(agentTCP, SIGNAL(StateConnecting(const QString)), this, SLOT(StateConnecting(const QString)));

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
        int size = 5;
        QRectF rectangle(width() -(width() / size) -10,
                         height() - (height() / size) - 10,
                         (width() / size),
                         (height() / size));

        int startAngle = 0 * 16;
        int spanAngle;

        if (currentPercentOfSteps ==0)
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
    Traces() << "\n" << "LOG: void CheckerArea::PaintMessage(QPainter *painter)";

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
        if (board->IsBlackPawnOnPos(x,y))
        {
            grabbed = board->GetBlackPawnNumber(x,y);

            if (possibleMoves.CanIGrab(grabbed, *board))
            {
                cursorState = Grab;
            };
        };
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
                            Traces() << "\n" << "cursorState = WaitForIA";
                            previousBoard = *board;
                            StartThinking();
                        } else cursorState = Free;
                    };
                }
                else
                {
                    if (board->GetNumberOfWhite()>0)
                    {
                        Traces() << "\n" << "cursorState = WaitForIA";
                        previousBoard = *board;
                        StartThinking();
                    } else cursorState = Free;
                };
            } else
            {
                if (board->GetNumberOfWhite()>0)
                {
                    Traces() << "\n" << "cursorState = WaitForIA";
                    board->SetBlackPawnPos(grabbed,x,y);
                    previousBoard = *board;
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

void CheckerArea::GetServerState(const ServerState &state)
{
    Traces() << "\n" << "LOG: void CheckerArea::GetServerState(const ServerState &state)";

    serverState = state;
    *board = serverState.GetBoard();
    repaint();
}

void CheckerArea::CheckStatus()
{

    repaint();
}

void CheckerArea::StateConnecting(const QString data)
{
    Traces() << "\n" << "LOG: void CheckerArea::StateConnecting(const QString data)";
    messageWindow = "Connecting to: " + data;
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
    delete ui;
}
