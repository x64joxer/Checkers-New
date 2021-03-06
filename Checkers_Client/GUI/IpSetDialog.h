#ifndef IPSETDIALOG_H
#define IPSETDIALOG_H

#include <QDialog>
#include "../ProgramVariables/ProgramVariables.h"

namespace Ui {
class IpSetDialog;
}

class IpSetDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit IpSetDialog(QWidget *parent = 0);
        ~IpSetDialog();

    signals:
        void IpChanged(const QString ip, const int port);

    private slots:
        void on_OKButton_released();
        void on_CancelButton_released();

    private:
        void showEvent(QShowEvent *event);

        Ui::IpSetDialog *ui;
};

#endif // IPSETDIALOG_H
