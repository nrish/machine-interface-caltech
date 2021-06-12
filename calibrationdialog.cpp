#include "calibrationdialog.h"
#include "ui_calibrationdialog.h"
#include <QObject>
#include <stdint.h>
#include <string>
CalibrationDialog::CalibrationDialog(QWidget *parent, SerialManager& serialManager) :
    QDialog(parent), serialManager(serialManager),
    ui(new Ui::CalibrationDialog)
{
    connect(&serialManager, &SerialManager::statusUpdate, this, &CalibrationDialog::serialStatusUpdate);
    ui->setupUi(this);
    ui->frame->setDisabled(true);
    ui->TrayX->setRange(0,5000);
    ui->TrayY->setRange(0,5000);
    ui->wellXDist->setRange(0, 5000);
    ui->wellYDist->setRange(0, 5000);
    StartDataSerialized startDataSerialized;
    startDataSerialized.startData.start_mode = 1;
    serialManager.sendData(startDataSerialized.bytes, sizeof(StartData));

    //wait for values to be recieved
    serialManager.getSerialPort().waitForReadyRead();

    CalibrationValueSerialized serializedValues;
    auto buff = serialManager.getData(sizeof(CalibrationValues));

    strcpy(buff.data(), (char*)serializedValues.bytes);
    qDebug() << "SerialData recieved"
             << serializedValues.values.WELL_DIST_X
             << serializedValues.values.WELL_DIST_Y
             << serializedValues.values.X_END_DIR
             << serializedValues.values.Y_END_DIR;
    this->values = serializedValues.values;

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
    this->values.WELL_DIST_X = (int16_t)ui->wellXDist->value();
    this->values.WELL_DIST_Y = (int16_t)ui->wellYDist->value();
    this->values.X_END_DIR = (bool)ui->xAxisDir->isChecked();
    this->values.Y_END_DIR = (bool)ui->yAxisDir->isChecked();
    CalibrationValueSerialized serializedValues;
    serializedValues.values = this->values;
    serialManager.sendData(serializedValues.bytes, sizeof(CalibrationValues));

}


void CalibrationDialog::on_traySelect_currentIndexChanged(int index)
{
    ui->TrayX->setValue(this->values.trays[index].x);
    ui->TrayY->setValue(this->values.trays[index].y);
}


void CalibrationDialog::on_TrayX_valueChanged(int i)
{
    this->values.trays[ui->traySelect->currentIndex()].x = i;
}


void CalibrationDialog::on_TrayY_valueChanged(int i)
{
    this->values.trays[ui->traySelect->currentIndex()].y = i;
}

void CalibrationDialog::serialStatusUpdate()
{

}

