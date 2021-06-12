#include "serialmanager.h"
#include "exception"
#include "string"
#include <mutex>

SerialManager::SerialManager(QSerialPortInfo port)
{
    sock.setPort(port);
    connect(&this->sock, &QSerialPort::readyRead, this, &SerialManager::dataRecieved);
    connect(&this->sock, &QSerialPort::errorOccurred, this, &SerialManager::onError);

}

void SerialManager::run()
{
    sock.setParity(QSerialPort::NoParity);
    sock.setStopBits(QSerialPort::OneStop);
    sock.setFlowControl(QSerialPort::NoFlowControl);
    sock.setBaudRate(QSerialPort::Baud19200);
    sock.setDataBits(QSerialPort::Data8);
    if (sock.open(QIODevice::ReadWrite)) {
        while(!this->isInterruptionRequested()){
            if(sock.bytesAvailable() > 0){
                buff.append(sock.readAll());
            }
        }
    }else{
        //attempt reconnect?
        emit connectionTerminated();
    }
    //wait for connected signal
}


bool SerialManager::serialConnected()
{
    return sock.isOpen();
}

QSerialPort& SerialManager::getSerialPort(){
    return this->sock;
}

SerialManager::~SerialManager()
{

}

void SerialManager::disconnect()
{
    this->sock.disconnect();
}

void SerialManager::dataRecieved(){
    buff.append(sock.readAll());
}

void SerialManager::onError(QSerialPort::SerialPortError){
    //?
}

void SerialManager::onAboutToClose()
{
    //?
}
