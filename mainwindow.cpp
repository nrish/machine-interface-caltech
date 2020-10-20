#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <string>
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
QSerialPort port;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->progressBar->setValue(0);
    ui->endWell->setRange(0,96);
    ui->startWell->setRange(0, 96);
    ui->inputBox->setEnabled(false);
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
    uint16_t startWell = ui->startWell->value();
    uint16_t endWell = ui->endWell->value();
    uint32_t timePerWell = std::stoul(ui->timeInput->toPlainText().toStdString());
    //start + end well have 4, timer per well has 4
    char bytes[10];
    ((uint32_t*)(bytes))[0] = timePerWell;
    ((uint16_t*)(bytes))[2] = startWell;
    ((uint16_t*)(bytes))[3] = endWell;
    ((uint16_t*)(bytes))[4] = (uint16_t)ui->trayCombo->currentIndex();
    port.write(bytes, 10);
    //wait for machine to reply
    port.waitForReadyRead(1000);
    port.readAll();

}
void MainWindow::on_connect_clicked()
{
    ui->inputBox->setEnabled(true);
    port.setPort(QSerialPortInfo(ui->portCombo->currentText()));
    port.setBaudRate(QSerialPort::Baud19200);
    port.setDataBits(QSerialPort::Data8);
    port.open(QIODevice::ReadWrite);
    if(port.isOpen()){
        ui->inputBox->setEnabled(true);
    }
}
