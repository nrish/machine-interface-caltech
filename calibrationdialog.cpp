#include "calibrationdialog.h"
#include "ui_calibrationdialog.h"
#include <QObject>
#include <stdint.h>
CalibrationDialog::CalibrationDialog(QWidget *parent, SerialManager& serialManager) :
    QDialog(parent), serialManager(serialManager),
    ui(new Ui::CalibrationDialog)
{
    connect(&serialManager, &SerialManager::statusUpdate, this, &CalibrationDialog::serialStatusUpdate);
    ui->setupUi(this);
    ui->frame->setDisabled(true);
    ui->TrayX->setRange(0,5000);
    ui->TrayY->setRange(0,5000);
    ui->trayXDist->setRange(0,5000);
    ui->trayYDist->setRange(0,5000);
    ui->wellXDist->setRange(0, 5000);
    ui->wellYDist->setRange(0, 5000);
    this->doneLoading = false;
    StartData data;
    data.start_mode = 1;
    StartDataSerialized startDataSerialized;
    startDataSerialized.startData = data;
    serialManager.sendData(startDataSerialized.bytes, sizeof(StartData));
    serialManager.getSerialPort().waitForReadyRead();
    EEPROMData serializedValues;
    serializedValues.values = values;
    serialManager.getData(serializedValues.bytes, sizeof(CalibrationValues));
    this->doneLoading = true;
    ui->trayXDist->setValue(values.TRAY_DIST_X);
    ui->trayYDist->setValue(values.TRAY_DIST_Y);
    ui->wellXDist->setValue(values.WELL_DIST_X);
    ui->wellYDist->setValue(values.WELL_DIST_Y);
    ui->xAxisDir->setChecked(values.X_END_DIR);
    ui->yAxisDir->setChecked(values.Y_END_DIR);
    ui->frame->setEnabled(true);
}

CalibrationDialog::~CalibrationDialog()
{
    delete ui;
}

void CalibrationDialog::on_DialogButtons_accepted()
{
    this->values.TRAY_DIST_X = (uint16_t)ui->trayXDist->value();
    this->values.TRAY_DIST_Y = (uint16_t)ui->trayYDist->value();
    this->values.WELL_DIST_X = (uint16_t)ui->wellXDist->value();
    this->values.WELL_DIST_Y = (uint16_t)ui->wellYDist->value();
    this->values.X_END_DIR = (bool)ui->xAxisDir->isChecked();
    this->values.Y_END_DIR = (bool)ui->yAxisDir->isChecked();
    EEPROMData serializedValues;
    serializedValues.values = values;
    serialManager.sendData(serializedValues.bytes, sizeof(CalibrationValues));

}


void CalibrationDialog::on_traySelect_currentIndexChanged(int index)
{
    ui->TrayX->setValue(values.trays[index].x);
    ui->TrayY->setValue(values.trays[index].y);
}


void CalibrationDialog::on_TrayX_valueChanged(int i)
{
    values.trays[ui->traySelect->currentIndex()].x = (uint8_t)ui->TrayX->value();
}


void CalibrationDialog::on_TrayY_valueChanged(int i)
{
    values.trays[ui->traySelect->currentIndex()].x = (uint8_t)ui->TrayX->value();
}

void CalibrationDialog::serialStatusUpdate()
{

}

