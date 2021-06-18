
#include "mainwindow.h"
#include "uint32spinbox.h"
#include "ui_mainwindow.h"
#include "imagedialog.h"
#include <string>
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QAction>
#include "serialmanager.h"
#include "serialData/serialData.h"
#include "calibrationdialog.h"
SerialManager serialManager;
CalibrationValueSerialized calibrationData;
int tally = 0;
int totalWells = 0;
MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    qDebug() << sizeof(CalibrationValues);
    ui->setupUi(this);
    ui->frame->setEnabled(false);
    //connect(ui->actionWell_Layout, QOverload<bool>::of(&QAction::triggered), this, &MainWindow::on_actionTrayWell_triggered);
    connect(&serialManager, &SerialManager::calibrationDataRecieved, this, &MainWindow::calibrationDataUpdate);
    connect(&serialManager, &SerialManager::updateDataRecieved, this, &MainWindow::updateDataRecieved);
    connect(&serialManager, &SerialManager::connected, this, &MainWindow::SerialConnected);
    connect(&serialManager, &SerialManager::connectionTerminated, this, &MainWindow::SerialConnectionTerminated);
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        qDebug() << "Name : " << info.portName();
        qDebug() << "Description : " << info.description();
        ui->portCombo->addItem(info.portName());
     }
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startButton_clicked()
{
        tally = 0;
        ui->progressBar->setValue(0);
        StartData startData;
        startData.endWellIndex = ui->endWell->value();
        startData.mills = ui->spinTimeWell->value();
        startData.trayIndex = ui->trayCombo->currentIndex();
        StartDataSerialized serialized = startData;
        serialManager.sendCommand(expect(CMD_STARTDATA, false), serialized.bytes);
}

void MainWindow::on_stopButton_clicked()
{
    //?
}

void MainWindow::on_refreshButton_clicked()
{
    ui->portCombo->clear();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        qDebug() << "Name : " << info.portName();
        qDebug() << "Description : " << info.description();
        ui->portCombo->addItem(info.portName());
     }
}

void MainWindow::calibrationDataUpdate(CalibrationValueSerialized data)
{
    calibrationData = data;
    auto dialog = new CalibrationDialog(this, &serialManager, &calibrationData);
    dialog->exec();
    delete dialog;
}

void MainWindow::updateDataRecieved(updateDataSerialized data)
{
    auto dialog = new QMessageBox();
    dialog->setText("calibrated");
    dialog->exec();
}

void MainWindow::SerialConnectionTerminated(QString error)
{
    auto dialog = new QMessageBox();
    dialog->setText("Serial connection closed: " + error);
    dialog->exec();
    ui->frame->setEnabled(false);
}

void MainWindow::SerialConnected()
{
    auto dialog = new QMessageBox();
    dialog->setText("Serial connection opened");
    dialog->exec();
    ui->frame->setEnabled(true);
}


void MainWindow::on_connectButton_clicked()
{
    auto serialport = QSerialPortInfo(ui->portCombo->currentText());
    serialManager.connectToPort(serialport);
    ui->connectButton->setEnabled(false);
}



void MainWindow::on_calibrateButton_clicked()
{
    serialManager.sendRequest(expect(CMD_CALIBRATION, true));
}


void MainWindow::on_actionWell_Layout_triggered()
{
    auto dialog = new ImageDialog(this, ":/images/96-well-temp-diagram.png", "Well Diagram");
    dialog->exec();
    delete dialog;
}

