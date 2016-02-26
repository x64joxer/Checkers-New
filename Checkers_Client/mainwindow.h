#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Board/Board.h"
#include "GUI/CheckerArea.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

    private:
        CheckerArea *checkerArea;
        Board *board;

        void resizeEvent( QResizeEvent *);

        Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
