#ifndef IAMAXTIMEDIALOG_H
#define IAMAXTIMEDIALOG_H

#include <QDialog>
#include "../ProgramVariables/ProgramVariables.h"

namespace Ui {
class IAMaxTimeDialog;
}

    class IAMaxTimeDialog : public QDialog
    {
        Q_OBJECT

    public:
        explicit IAMaxTimeDialog(QWidget *parent = 0);
        ~IAMaxTimeDialog();

    private slots:
        void on_OKButton_released();
        void on_CancelButton_released();

    private:
        void showEvent(QShowEvent *event);

        Ui::IAMaxTimeDialog *ui;
};

#endif // IAMAXTIMEDIALOG_H
