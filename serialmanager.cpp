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

void SerialManager::sendRequest(expect e)
{
    expectSerialized cmd = e;
    sock.write((char*)cmd.bytes, sizeof(expect));
    sock.flush();
}

void SerialManager::sendCommand(expect e, QByteArray bytes)
{
    expectSerialized cmd = e;
    sock.write((char*)cmd.bytes, sizeof(expect));
    sock.write(bytes.data(), bytes.size());
    sock.flush();
}

void SerialManager::connectToPort(QSerialPortInfo port)
{
    if(this->connectionStatus){
        this->sock.close();
    }
    sock.setPort(port);
    sock.setParity(QSerialPort::NoParity);
    sock.setStopBits(QSerialPort::OneStop);
    sock.setFlowControl(QSerialPort::NoFlowControl);
    sock.setBaudRate(QSerialPort::Baud19200);
    sock.setDataBits(QSerialPort::Data8);
    if(sock.open(QIODevice::ReadWrite)){
        emit connected();
    }
}

void SerialManager::disconnect()
{
    this->sock.disconnect();
}

void SerialManager::dataRecieved(){
    buff.append(sock.readAll());
    qDebug() << "got data, bytes: " << buff.size();
    while(!buff.isEmpty()){
        //we might have a command
        if(buff.size() < sizeof(expect))
            break;
        expectSerialized expectData;
        for(uint32_t i = 0; i < sizeof(expect); i++){
            expectData.bytes[i] = buff.data()[i];
        }

        //make sure there are enough bytes, if not wait for more data.
        if(expectData.values.bytes > buff.size())
            break;
        //first remove expect data
        buff.remove(0, sizeof(expect));
        if(expectData.values.request){
            //nothing to really do here yet.
        }else{
            if(expectData.values.cmd == CMD_STARTDATA){
                //not expecting this
            } else if(expectData.values.cmd == CMD_CALIBRATION){
                CalibrationValueSerialized calData;
                for(uint32_t i = 0; i < sizeof(CalibrationValues); i++){
                    expectData.bytes[i] = buff.data()[i];
                }
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
        }
        buff.remove(0,expectData.values.bytes);
    }
}

void SerialManager::onError(QSerialPort::SerialPortError error){
    //?
    if(error == QSerialPort::SerialPortError::NoError){
        //shouldn't be here?
    }else {
        emit connectionTerminated(sock.errorString());
    }
}

void SerialManager::onAboutToClose()
{
    //?
}

