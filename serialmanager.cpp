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
}

void SerialManager::getData(uint8_t *buf, int len)
{
    for(int i = 0; i < len; i++){
        buf[i] = log.front();
        log.pop();
    }
}

void SerialManager::connectToPort(QSerialPortInfo port){
    conn.setPort(port);
    conn.setBaudRate(QSerialPort::Baud19200);
    conn.setDataBits(QSerialPort::Data8);
    if(!QSerialPortInfo(port).isBusy()){
        conn.open(QIODevice::ReadWrite);
        this->connected = true;
    }
}

int SerialManager::getBytesReady()
{
    return log.size();
}
void SerialManager::dataRecieved(){
    char data;
    char* ptr = &data;
    //write data into log queue until nothing is left.
    while(!conn.atEnd()){
        conn.read(ptr, 1);
        log.push(data);
    }
}

void SerialManager::onError(QSerialPort::SerialPortError){
    this->connected = false;
    conn.close();
}

void SerialManager::onAboutToClose()
{
    this->connected = false;
}
