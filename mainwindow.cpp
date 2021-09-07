#include "mainwindow.h"
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
#include "traysequenceitem.h"
#include "addtraydialog.h"


DeviceManager deviceManager;
MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->groupBoxSettings->setEnabled(false);
    connect(&deviceManager, &DeviceManager::connected, this, &MainWindow::SerialConnected);
    connect(&deviceManager, &DeviceManager::connectionTerminated, this, &MainWindow::SerialConnectionTerminated);
    connect(&deviceManager, &DeviceManager::calibrationDataRecieved, this, &MainWindow::on_calibration_recieve);
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        qDebug() << "Name : " << info.portName();
        qDebug() << "Description : " << info.description();
        ui->portCombo->addItem(info.portName());
     }
    ui->trayList->setSelectionMode(QAbstractItemView::MultiSelection);
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startButton_clicked()
{
    deviceManager.clearTrays();
    for(int i = 0; i < ui->trayList->count(); i++){
        TraySequenceItem* item = (TraySequenceItem*)ui->trayList->item(i);
        qDebug() << "sending tray item #" << i;
        deviceManager.addTray(item->getIndex(), item->getTime(), item->getStartWell(), item->getEndWell());
    }
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
    if(error != NULL)
        dialog->setText("Serial connection closed unexpectedly: " + error);
    else
        dialog->setText("Serial connection closed.");
    dialog->exec();
    delete dialog;
    ui->groupBoxSettings->setEnabled(false);
}

void MainWindow::SerialConnected()
{
    ui->groupBoxSettings->setEnabled(true);
    ui->trayList->clear();
}


void MainWindow::on_connectButton_clicked()
{
    auto serialport = QSerialPortInfo(ui->portCombo->currentText());
    deviceManager.connectToPort(serialport);
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


void MainWindow::on_tray_add_clicked()
{
    auto dialog = new addTrayDialog(this);
    dialog->exec();
    delete dialog;
}

void MainWindow::newTraySequence(int time, int startwell, int endwell, int index)
{
    if(startwell > endwell)
        QMessageBox::information(this, "Invalid tray configuration", "Invalid tray configuration, first well must be before last well");
    for(int i = 0; i < ui->trayList->count(); i++){
        if(((TraySequenceItem*)ui->trayList->item(i))->getIndex() == index){
            auto keepTray = QMessageBox::question(this, "Tray In Use","Tray already in use, replace old tray?", QMessageBox::Yes | QMessageBox::No);
            if(keepTray == QMessageBox::Yes){
                delete ui->trayList->takeItem(i);
            }
        }
    }
    auto item = new TraySequenceItem(time, startwell, endwell, index);
    ui->trayList->addItem(item);
}


void MainWindow::on_tray_remove_clicked()
{
    for(auto i : ui->trayList->selectedItems()){
        ui->trayList->removeItemWidget(i);
        delete i;
    }
}

