#include "serialmanager.h"
#include "exception"
#include "string"
#include <mutex>

SerialManager::SerialManager()
{
    connect(&this->sock, &QSerialPort::readyRead, this, &SerialManager::dataRecieved);
    connect(&this->sock, &QSerialPort::errorOccurred, this, &SerialManager::onError);
}
SerialManager::~SerialManager(){
    this->connectionStatus = false;
}

int SerialManager::exec()
{
    qDebug() <<"u";
    threadMutex.lock();
    qDebug() <<"l";
    sock.setParity(QSerialPort::NoParity);
    sock.setStopBits(QSerialPort::OneStop);
    sock.setFlowControl(QSerialPort::NoFlowControl);
    sock.setBaudRate(QSerialPort::Baud19200);
    sock.setDataBits(QSerialPort::Data8);
    if (sock.open(QIODevice::ReadWrite)) {
        threadMutex.unlock();
        emit connected();
        this->connectionStatus = true;
        while(this->connectionStatus && !this->errorState){
            qDebug() <<"test";
            if(!this->commandQueue.isEmpty()){
                this->threadMutex.lock();
                auto command = commandQueue.first();
                sock.write(command.data(), command.size());
                commandQueue.removeFirst();
                this->threadMutex.unlock();
            }
            if(buff.size() > sizeof(expect)){
                this->threadMutex.lock();
                //first get expected
                expectSerialized expectData;
                for(uint32_t i = 0; i < sizeof(expect); i++){
                    expectData.bytes[i] = buff.data()[i];
                }
                uint32_t expectedBytes = expectData.values.bytes;
                //wait for these bytes to arrive
                while (sock.bytesAvailable() < expectedBytes);
                //put all bytes in buff
                buff.append(sock.readAll());
                //read new symbol
                //onlt get data here
                if(expectData.values.cmd == CMD_STARTDATA){
                    //nothing to do

                } else if(expectData.values.cmd == CMD_CALIBRATION){
                    //we only get calibration data here
                    CalibrationValueSerialized calData;
                    for(uint32_t i = 0; i < sizeof(calData); i++){
                        expectData.bytes[i] = buff.data()[i];
                    }
                    //effectively const
                    emit calibrationDataRecieved(calData);
                } else if( expectData.values.cmd == CMD_UPDATE){
                    //emit update!
                    updateDataSerialized upData;
                    for(uint32_t i = 0; i < sizeof(upData); i++){
                        expectData.bytes[i] = buff.data()[i];
                    }
                    //effectively const
                    emit updateDataRecieved(upData);

                }else if(expectData.values.cmd == CMD_SETPOS){
                    //nothing to do

                }else{
                    //unknown command
                }
                this->threadMutex.unlock();
            }
        }
    }else{
        qDebug() << "exit";
        sock.close();
        threadMutex.unlock();
        emit connectionTerminated(sock.error());
    }
    //
    threadMutex.lock();
    sock.close();
    threadMutex.unlock();
    emit connectionTerminated(sock.error());
}


void SerialManager::queueCommand(QByteArray data)
{
    this->threadMutex.lock();
    commandQueue.append(data);
    this->threadMutex.unlock();
}

void SerialManager::connectToPort(QSerialPortInfo port)
{
    threadMutex.lock();
    if(this->connectionStatus){
        this->sock.close();
        this->connectionStatus = false;
    }
    sock.setPort(port);
    this->threadMutex.unlock();
    this->start();
}

const QSerialPort& SerialManager::getSerialPort() const{
    return this->sock;
}

void SerialManager::disconnect()
{
    threadMutex.lock();
    this->sock.disconnect();
    threadMutex.unlock();
}

void SerialManager::dataRecieved(){
    threadMutex.lock();
    buff.append(sock.readAll());
    threadMutex.unlock();
}

void SerialManager::onError(QSerialPort::SerialPortError error){
    //?
    if(error == QSerialPort::SerialPortError::NoError){
        //shouldn't be here?
    }else {
        this->errorState = true;
    }
}

void SerialManager::onAboutToClose()
{
    //?
}

bool SerialManager::isConnected() const
{
    return connectionStatus;
}
