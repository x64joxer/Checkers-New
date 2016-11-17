#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Board/Board.h"
#include "GUI/CheckerArea.h"
#include "../Common/Traces/Traces.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

private slots:
    void on_actionNew_triggered();

private:
        CheckerArea *checkerArea;
        Board *board;

        void resizeEvent( QResizeEvent *);

        Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
