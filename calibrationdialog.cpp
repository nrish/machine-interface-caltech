#include "calibrationdialog.h"
#include "ui_calibrationdialog.h"
#include <QObject>
#include <stdint.h>
#include <string>
#include <QSpinBox>

CalibrationDialog::CalibrationDialog(QWidget *parent, DeviceManager* deviceManager) :
    QDialog(parent),
    ui(new Ui::CalibrationDialog)
{
    this->deviceManager = deviceManager;
    ui->setupUi(this);
    ui->frame->setDisabled(true);
    ui->TrayX->setRange(0, INT_MAX);
    ui->TrayY->setRange(0, INT_MAX);
    ui->wellXDist->setRange(0, 5000);
    ui->wellYDist->setRange(0, 5000);
    ui->wellXDist->setValue(deviceManager->getCalibrationValues().getWellDistX());
    ui->wellYDist->setValue(deviceManager->getCalibrationValues().getWellDistY());
    ui->xAxisDir->setChecked(deviceManager->getCalibrationValues().getXAxisDir());
    ui->yAxisDir->setChecked(deviceManager->getCalibrationValues().getYAxisDir());
    ui->frame->setEnabled(true);
}
CalibrationDialog::~CalibrationDialog()
{
    delete ui;
}

void CalibrationDialog::on_DialogButtons_accepted()
{
    deviceManager->getCalibrationValues().setWellDistX(ui->wellXDist->value());
    deviceManager->getCalibrationValues().setWellDistY(ui->wellYDist->value());
    deviceManager->getCalibrationValues().setXAxisDir(ui->xAxisDir->isChecked());
    deviceManager->getCalibrationValues().setYAxisDir(ui->yAxisDir->isChecked());
    CalibrationValues & values = deviceManager->getCalibrationValues();
    deviceManager->setDistCalibration(values.well_dist_x, values.well_dist_y, values.x_axis_dir, values.y_axis_dir);
    for(int i = 0; i < 8; i++)
        deviceManager->setTrayCalibration(i, values.getTray(i).xpos, values.getTray(i).ypos);
    deviceManager->confirmCalibration();
}


void CalibrationDialog::on_traySelect_currentIndexChanged(int index)
{
    qDebug() << index;
    ui->TrayX->setValue(deviceManager->getCalibrationValues().getTray(index).xpos);
    ui->TrayY->setValue(deviceManager->getCalibrationValues().getTray(index).ypos);
}


void CalibrationDialog::on_TrayX_valueChanged(int i)
{
    deviceManager->getCalibrationValues().getTray(ui->traySelect->currentIndex()).xpos = i;
}


void CalibrationDialog::on_TrayY_valueChanged(int i)
{
    deviceManager->getCalibrationValues().getTray(ui->traySelect->currentIndex()).ypos = i;
}

void CalibrationDialog::serialStatusUpdate()
{

}


void CalibrationDialog::on_testPos_clicked()
{
    auto currentTray = deviceManager->getCalibrationValues().getTray(ui->traySelect->currentIndex());
    deviceManager->setPos(currentTray.xpos, currentTray.ypos);
}


void CalibrationDialog::on_homeButton_clicked()
{
    deviceManager->home();
}

