#include "calibrationdialog.h"
#include "ui_calibrationdialog.h"
#include <QObject>
#include <stdint.h>
#include <string>
CalibrationDialog::CalibrationDialog(QWidget *parent, SerialManager* serialManager, CalibrationValueSerialized *calibration) :
    QDialog(parent),
    ui(new Ui::CalibrationDialog)
{
    this->serialManager = serialManager;
    this->calibration = calibration;

    ui->setupUi(this);
    ui->frame->setDisabled(true);
    ui->TrayX->setRange(0,5000);
    ui->TrayY->setRange(0,5000);
    ui->wellXDist->setRange(0, 5000);
    ui->wellYDist->setRange(0, 5000);

    ui->wellXDist->setValue(this->calibration->values.WELL_DIST_X);
    ui->wellYDist->setValue(this->calibration->values.WELL_DIST_Y);
    ui->xAxisDir->setChecked(this->calibration->values.X_END_DIR);
    ui->yAxisDir->setChecked(this->calibration->values.Y_END_DIR);
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
    expectSerialized commandIssue(expect(sizeof(CalibrationValues), CMD_CALIBRATION, false));
    CalibrationValueSerialized serializedValues;
    serializedValues.values = this->values;
    serialManager->queueCommand(QByteArray((char*)serializedValues.bytes, sizeof(CalibrationValues)));

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

