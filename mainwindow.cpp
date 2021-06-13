
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
SerialManager *serialManager = nullptr;
CalibrationValueSerialized *calibrationData;
int tally = 0;
int totalWells = 0;

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    qDebug() << sizeof(CalibrationValues);
    ui->setupUi(this);
    ui->frame->setEnabled(false);
    connect(ui->actionWell_Layout, QOverload<bool>::of(&QAction::triggered), this, &MainWindow::on_actionTrayWell_triggered);
    connect(serialManager, &SerialManager::calibrationDataRecieved, this, &MainWindow::calibrationDataUpdate);
    connect(serialManager, &SerialManager::updateDataRecieved, this, &MainWindow::updateDataRecieved);
    connect(serialManager, &SerialManager::connected, this, &MainWindow::SerialConnected);
    connect(serialManager, &SerialManager::connectionTerminated, this, &MainWindow::SerialConnectionTerminated);

    connect(ui->pushConnect, &QPushButton::clicked, this, &MainWindow::on_connectButton_clicked);
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        qDebug() << "Name : " << info.portName();
        qDebug() << "Description : " << info.description();
        ui->portCombo->addItem(info.portName());
     }
}


MainWindow::~MainWindow()
{
    if(serialManager != nullptr)
        delete serialManager;
    delete ui;
}

void MainWindow::on_startButton_clicked()
{
    if(serialManager != nullptr && serialManager->isConnected()){
        tally = 0;
        ui->progressBar->setValue(0);
        StartData startData;
        StartDataSerialized serialized;
        startData.endWellIndex = ui->endWell->value();
        startData.mills = ui->spinTimeWell->value();
        startData.trayIndex = ui->trayCombo->currentIndex();
        serialized.values = startData;
        serialManager->queueCommand(QByteArray((char*)serialized.bytes));
    }
}

void MainWindow::on_stopButton_clicked()
{
    //?
}

void MainWindow::on_actionTrayWell_triggered(bool)
{
    auto dialog = new ImageDialog(this, ":/images/96-well-temp-diagram.png", "Well Diagram");
    dialog->exec();
    delete dialog;
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
    calibrationData->values = data.values;
}

void MainWindow::updateDataRecieved(updateDataSerialized data)
{
    //handle progress bar
}

void MainWindow::SerialConnectionTerminated(QSerialPort::SerialPortError error)
{
    auto dialog = new QMessageBox();
    dialog->setText("Serial connection closed");
    dialog->exec();
    ui->frame->setEnabled(false);
}

void MainWindow::SerialConnected()
{
    auto dialog = new QMessageBox();
    dialog->setText("Serial connection closed");
    dialog->exec();
    ui->frame->setEnabled(true);
}


void MainWindow::on_connectButton_clicked()
{
    qDebug() << "test";
    if(serialManager != nullptr)
        delete serialManager;
    auto serialport = QSerialPortInfo(ui->portCombo->currentText());
    serialManager = new SerialManager();
    serialManager->connectToPort(serialport);
}



void MainWindow::on_calibrateButton_clicked()
{
    if(serialManager != nullptr && serialManager->isConnected()){
        auto dialog = new CalibrationDialog(this, serialManager, calibrationData);
        dialog->exec();
        delete dialog;
    }
}

