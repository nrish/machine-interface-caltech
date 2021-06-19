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

void SerialManager::sendCommand(expect e, uint8_t* bytes)
{
    sendRequest(e);
    this->writePacketStream((char*)bytes, e.bytes);
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

void SerialManager::writePacketStream(char* bytes, size_t len){
    //get number of packets needed to send data plus one if it doesn't fit evenly
    char conf = 0;
    qDebug() << "writing";
    int packets = len/16 + (len%16 == 0 ? 0 : 1);
    for(int i = 0; i < packets; i++){
        //copy 16 byte chunks and write
        sock.write((bytes+i*16), 16);
        sock.flush();
        //wait for confirm bit
        sock.waitForReadyRead();
        sock.skip(1);
    }
    qDebug() << "writing";
}
void SerialManager::readPacketStream(QByteArray &arry, size_t len){
    char packet[16];
    int packets = len/16 + (len%16 == 0 ? 0 : 1);
//    char sig = 1;
    for(int i = 0; i < packets; i++){
        //read 16 byte chunk
        sock.read(packet, 16);
        arry.append(packet, 16);
//        //write confirm bit
//        qDebug() << "waiting for recieve";
//        sock.write(&sig, 1);
    }
}
void SerialManager::dataRecieved(){
//    buff.append(sock.readAll());
//    qDebug() << "got data, bytes: " << buff.size();
//    qDebug() << QString(buff);
    qDebug() << "got " << sock.bytesAvailable();
    expectSerialized expectData;
    int len = sock.peek((char*)expectData.bytes, sizeof(expect));
    if(len >= sizeof(expect) && sock.bytesAvailable() >= expectData.values.bytes+sizeof(expect)){
        sock.skip(sizeof(expect));
        QByteArray arry;
        readPacketStream(arry, expectData.values.bytes);
        qDebug() << "arry size (cal): " << arry.size();
        if(expectData.values.request){
            //nothing to really do here yet.
        }else{
            if(expectData.values.cmd == CMD_STARTDATA){
                //not expecting this
            } else if(expectData.values.cmd == CMD_CALIBRATION){
                CalibrationValueSerialized calData;
                memcpy(calData.bytes, arry.data(), sizeof(CalibrationValues));
                emit calibrationDataRecieved(calData);
            } else if( expectData.values.cmd == CMD_UPDATE){
                QByteArray arry;
                updateDataSerialized upData;
                memcpy(upData.bytes, arry.data(), sizeof(updateData));
                emit updateDataRecieved(upData);

            }else if(expectData.values.cmd == CMD_SETPOS){
                //nothing to do

            }else if(expectData.values.cmd == DEBUG_MSG){
                qDebug() << "got debug msg";
                qDebug() << QString::fromUtf8(arry).toUtf8();

            }else{
                //unknown command
            }
        }
        sock.readAll();
    }
}

void SerialManager::onError(QSerialPort::SerialPortError error){
    //?
    if(error == QSerialPort::SerialPortError::NoError){
        //shouldn't be here?
    }else {
        this->sock.close();
        emit connectionTerminated(sock.errorString());
    }
}

void SerialManager::onAboutToClose()
{
    emit connectionTerminated("");
}

