
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
#include "deviceManager.h"
#include "calibrationdialog.h"


DeviceManager deviceManager;
MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->frame->setEnabled(false);
    connect(&deviceManager, &DeviceManager::connected, this, &MainWindow::SerialConnected);
    connect(&deviceManager, &DeviceManager::connectionTerminated, this, &MainWindow::SerialConnectionTerminated);
    connect(&deviceManager, &DeviceManager::calibrationDataRecieved, this, &MainWindow::on_calibration_recieve);
    ui->spinTimeWell->setRange(0, INT_MAX);
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
        deviceManager.start();
        ui->progressBar->setValue(0);
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

void MainWindow::on_calibration_recieve()
{
    auto dialog = new CalibrationDialog(this, &deviceManager);
    dialog->exec();
    delete dialog;
}


void MainWindow::SerialConnectionTerminated(QString error)
{
    auto dialog = new QMessageBox();
    if(error == NULL)
        dialog->setText("Serial connection closed unexpectedly: " + error);
    else
        dialog->setText("Serial connection closed.");
    dialog->exec();
    ui->connectButton->setEnabled(true);
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
    deviceManager.connectToPort(serialport);
    ui->connectButton->setEnabled(false);
}

void MainWindow::on_calibrateButton_clicked()
{
    deviceManager.getCalibrationData();
}


void MainWindow::on_actionWell_Layout_triggered()
{
    auto dialog = new ImageDialog(this, ":/images/96-well-temp-diagram.png", "Well Diagram");
    dialog->exec();
    delete dialog;
}

