#include "serialmanager.h"
#include "string"
SerialManager::SerialManager()
{
    this->connected = false;
    connect(&this->conn, &QSerialPort::readyRead, this, &SerialManager::dataRecieved);
    connect(&this->conn, &QSerialPort::errorOccurred, this, &SerialManager::onError);

}

void SerialManager::sendData(uint8_t *buf, int size)
{
    if(conn.isOpen() && conn.isWritable()){
        conn.write((char*)buf, size);
    }
    conn.flush();
}

void SerialManager::getData(uint8_t *buf, int len)
{
    conn.read((char*)buf, len);
}

void SerialManager::connectToPort(QSerialPortInfo port){
    conn.setPort(port);
    conn.setParity(QSerialPort::NoParity);
    conn.setStopBits(QSerialPort::OneStop);
    conn.setFlowControl(QSerialPort::NoFlowControl);
    conn.setBaudRate(QSerialPort::Baud19200);
    conn.setDataBits(QSerialPort::Data8);
    if(!QSerialPortInfo(port).isBusy()){
        conn.open(QIODevice::ReadWrite);
        this->connected = true;
    }
}

int SerialManager::getBytesReady()
{
    return conn.bytesAvailable();
}

bool SerialManager::serialConnected()
{
    return this->connected;
}

QSerialPort& SerialManager::getSerialPort(){
    return this->conn;
}

SerialManager::~SerialManager()
{

}

void SerialManager::disconnect()
{
    this->conn.disconnect();
}

void SerialManager::dataRecieved(){
    qDebug() << conn.bytesAvailable();
    emit statusUpdate();
}

void SerialManager::onError(QSerialPort::SerialPortError){
    this->connected = false;
}

void SerialManager::onAboutToClose()
{
    this->connected = false;
}
