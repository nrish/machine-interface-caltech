
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
#include "serialData.h"
#include "calibrationdialog.h"
SerialManager serialManager;
int tally = 0;
int totalWells = 0;

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    qDebug() << sizeof(CalibrationValues);
    ui->setupUi(this);
    ui->frame->setEnabled(false);
    connect(ui->actionWell_Layout, QOverload<bool>::of(&QAction::triggered), this, &MainWindow::on_actionTrayWell_triggered);
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
    StartDataSerialized serialized;
    startData.start_mode = 0;
    startData.endWellIndex = ui->endWell->value();
    startData.mills = ui->spinTimeWell->value();
    startData.tray_index = ui->trayCombo->currentIndex();
    serialized.startData = startData;
    serialManager.sendData(serialized.bytes, sizeof(startData));
}

void MainWindow::on_stopButton_clicked()
{
    uint8_t a = 0x03;
    serialManager.sendData(&a, 1);
}


void MainWindow::on_pushConnect_pressed()
{
    if(ui->portCombo->currentText() != ""){
        serialManager.connectToPort(QSerialPortInfo(ui->portCombo->currentText()));
        if(serialManager.serialConnected()){
            ui->frame->setEnabled(true);
        }
    }
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


void MainWindow::on_pushButton_clicked()
{
    auto dialog = new CalibrationDialog(this, serialManager);
    dialog->exec();
    delete dialog;
}

