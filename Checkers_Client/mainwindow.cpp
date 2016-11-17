#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Traces::SetTraceFolder("trace");

     //Create board
     board = new Board();
    *board =
                std::string("| |w| |w| |w| |w|") +
                std::string("|w| |w| |w| |w| |") +
                std::string("| |w| |w| |w| |w|") +
                std::string("| | | | | | | | |") +
                std::string("| | | | | | | | |") +
                std::string("|b| |b| |b| |b| |") +
                std::string("| |b| |b| |b| |b|") +
                std::string("|b| |b| |b| |b| |");

     //Create board area
    checkerArea = new CheckerArea(this);
    checkerArea->SetBoard(board);
}

void MainWindow::resizeEvent( QResizeEvent *)
{
    checkerArea->move(0,30);
    checkerArea->resize(width(),height()-30);
}

void MainWindow::on_actionNew_triggered()
{
    checkerArea->ResetServerState();
}

MainWindow::~MainWindow()
{
    delete checkerArea;
    delete board;
    delete ui;
}

