
#include "mainwindow.h"
#include "uint32spinbox.h"
#include "ui_mainwindow.h"
//#include "qresource.h"
#include "imagedialog.h"
#include <string>
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QAction>

QSerialPort port;
QSerialPortInfo info;
int tally = 0;
int totalWells = 0;

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->frame->setEnabled(false);

    connect(&port, &QSerialPort::readyRead, this, &MainWindow::onSerialReceivedDataSignal);
    connect(&port, &QSerialPort::aboutToClose, this, &MainWindow::onPortDisconnect);
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

void MainWindow::onSerialReceivedDataSignal(){
    char a = 0;
    port.read(&a, 1);
    if(a == 0x01){
        tally++;
        ui->progressBar->setValue(((float)tally/totalWells) * 100);
    }
}

void MainWindow::onPortDisconnect(){
    ui->frame->setEnabled(false);
}

void MainWindow::on_startButton_clicked()
{
    tally = 0;
    ui->progressBar->setValue(0);
    uint16_t endWell = ui->endWell->value();
    uint32_t timePerWell = ui->spinTimeWell->value();
    totalWells = ui->endWell->value();

    /*
    try{
        timePerWell = std::stoul(ui->timeInput->text().toStdString());
    }catch(std::exception e){
        QMessageBox message(this);
        message.setText("Invalid time input");
        message.setInformativeText("Make sure time input field contains numeral characters only.");
        message.setStandardButtons(QMessageBox::Ok);
        message.exec();
        return;
    }
    */

    //start + end well have 4, timer per well has 4
    char bytes[10];
    ((uint32_t*)(bytes))[0] = timePerWell;
    qDebug() << timePerWell;
    ((uint16_t*)(bytes))[3] = endWell;
    ((uint16_t*)(bytes))[4] = (uint16_t)ui->trayCombo->currentIndex();
    port.write(bytes, 10);
    //wait for machine to reply
    port.waitForReadyRead(1000);
    port.readAll();
}

void MainWindow::on_stopButton_clicked()
{
    char a = 0x03;
    port.write(&a, 1);
}


void MainWindow::on_pushConnect_pressed()
{
    if(ui->portCombo->currentText() != ""){

        ui->frame->setEnabled(true);
        port.setPort(QSerialPortInfo(ui->portCombo->currentText()));
        port.setBaudRate(QSerialPort::Baud19200);
        port.setDataBits(QSerialPort::Data8);
        if(!QSerialPortInfo(port).isBusy()){
            port.open(QIODevice::ReadWrite);
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
