#include "IpSetDialog.h"
#include "ui_IpSetDialog.h"

IpSetDialog::IpSetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IpSetDialog)
{
    ui->setupUi(this);
}

void IpSetDialog::on_OKButton_released()
{
    emit IpChanged(ui->IPlineEdit->displayText().remove(" "), ui->serverPort->displayText().toInt());
    close();
}

void IpSetDialog::on_CancelButton_released()
{
    close();
}

void IpSetDialog::showEvent(QShowEvent *event)
{
    ui->IPlineEdit->setText(ProgramVariables::GetServerIP());
    ui->serverPort->setText(QString::number(ProgramVariables::GetServerPort()));
}

IpSetDialog::~IpSetDialog()
{
    delete ui;
}
