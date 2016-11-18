#include "IAMaxTimeDialog.h"
#include "ui_IAMaxTimeDialog.h"

IAMaxTimeDialog::IAMaxTimeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IAMaxTimeDialog)
{
    ui->setupUi(this);
}

void IAMaxTimeDialog::showEvent(QShowEvent *event)
{
    ui->IAMaxTimeSpinBox->setValue(ProgramVariables::GetMaxMilisecondsForIa() / 1000);
}

void IAMaxTimeDialog::on_OKButton_released()
{
    ProgramVariables::SetMaxMilisecondsForIa(ui->IAMaxTimeSpinBox->text().toFloat() * 1000);
    close();
}

void IAMaxTimeDialog::on_CancelButton_released()
{
    close();
}

IAMaxTimeDialog::~IAMaxTimeDialog()
{
    delete ui;
}

