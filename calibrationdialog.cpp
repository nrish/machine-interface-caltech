#include "calibrationdialog.h"
#include "ui_calibrationdialog.h"
#include <QObject>
#include <stdint.h>
#include <string>
#include <QSpinBox>

CalibrationDialog::CalibrationDialog(QWidget *parent, SerialManager* serialManager, CalibrationValueSerialized *calibration) :
    QDialog(parent),
    ui(new Ui::CalibrationDialog)
{
    this->serialManager = serialManager;
    this->calibration = calibration;
    for(int i = 0; i < 8; i++)
        qDebug() << calibration->values.trays[i].x << " " << calibration->values.trays[i].y;
    ui->setupUi(this);
    ui->frame->setDisabled(true);
    ui->TrayX->setRange(0, INT_MAX);
    ui->TrayY->setRange(0, INT_MAX);
    ui->wellXDist->setRange(0, 5000);
    ui->wellYDist->setRange(0, 5000);
    ui->wellXDist->setValue(this->calibration->values.WELL_DIST_X);
    ui->wellYDist->setValue(this->calibration->values.WELL_DIST_Y);
    ui->xAxisDir->setChecked(this->calibration->values.X_END_DIR);
    ui->yAxisDir->setChecked(this->calibration->values.Y_END_DIR);
    targetTray = &calibration->values.trays[0];
    ui->TrayX->setValue(targetTray->x);
    ui->TrayY->setValue(targetTray->y);
    ui->frame->setEnabled(true);
}
CalibrationDialog::~CalibrationDialog()
{
    delete ui;
}

void CalibrationDialog::on_DialogButtons_accepted()
{
    this->calibration->values.WELL_DIST_X = (int16_t)ui->wellXDist->value();
    this->calibration->values.WELL_DIST_Y = (int16_t)ui->wellYDist->value();
    this->calibration->values.X_END_DIR = (bool)ui->xAxisDir->isChecked();
    this->calibration->values.Y_END_DIR = (bool)ui->yAxisDir->isChecked();
    serialManager->sendCommand(expect(CMD_CALIBRATION, false), calibration->bytes);

}


void CalibrationDialog::on_traySelect_currentIndexChanged(int index)
{
    qDebug() << index;
    this->targetTray = &calibration->values.trays[index];
    ui->TrayX->setValue(targetTray->x);
    ui->TrayY->setValue(targetTray->y);
}


void CalibrationDialog::on_TrayX_valueChanged(int i)
{
    qDebug() << ui->traySelect->currentIndex();
    targetTray->x = i;
    qDebug() << this->calibration->values.trays[ui->traySelect->currentIndex()].x;
}


void CalibrationDialog::on_TrayY_valueChanged(int i)
{
    qDebug() << ui->traySelect->currentIndex();
    targetTray->y = i;
    qDebug() << this->calibration->values.trays[ui->traySelect->currentIndex()].y;
}

void CalibrationDialog::serialStatusUpdate()
{

}


void CalibrationDialog::on_testPos_clicked()
{
    setPosSerialized cmd = setPos(targetTray->x, targetTray->y, true, false);
    serialManager->sendCommand(expect(CMD_SETPOS, false), cmd.bytes);
}


void CalibrationDialog::on_homeButton_clicked()
{
    setPosSerialized cmd = setPos(0, 0, false, true);
    serialManager->sendCommand(expect(CMD_SETPOS, false), cmd.bytes);
}

