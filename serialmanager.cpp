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
    sock.write((char*)bytes, e.bytes);
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

void SerialManager::writePacketStream(QByteArray arry){
  //get number of packets needed to send data plus one if it doesn't fit evenly
  int packets = arry.size()/16 + arry.size()%16;
  for(int i = 0; i < packets; i++){
    //copy 16 byte chunks and write
    sock.write((arry.data()+i*16), 16);
    //wait for confirm bit
    sock.waitForReadyRead();
    //read single confirm bit
    sock.read(null,0);
  }
}
void SerialManager::readPacketStream(QByteArray arry){

  int packets = len/16 + len%16;
  char sig = 1;
  for(int i = 0; i < packets; i++){
    //read 16 byte chunk
    sock.read((packet+i*16), 16);
    //write confirm bit
    sock.write(&sig, 1);
  }
}
void SerialManager::dataRecieved(){
    char first;
    sock.peek(&first, 1);
    if(first == '<'){
        //do stuff
        sock.AllDirections
    }else{
        //clear everything, something is wrong if we
        sock.readAll();
    }

    buff.append(sock.readAll());
    qDebug() << "got data, bytes: " << buff.size();
    qDebug() << QString(buff);
    while(!buff.isEmpty()){
        //we might have a command
        if(buff.size() < sizeof(expect))
            break;
        expectSerialized expectData;
        for(uint32_t i = 0; i < sizeof(expect); i++){
            expectData.bytes[i] = buff.data()[i];
        }

        //make sure there are enough bytes, if not wait for more data.
        if(buff.size() < expectData.values.bytes)
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
                    calData.bytes[i] = buff.data()[i];
                }
                emit calibrationDataRecieved(calData);
            } else if( expectData.values.cmd == CMD_UPDATE){
                //emit update!
                updateDataSerialized upData;
                for(uint32_t i = 0; i < sizeof(updateData); i++){
                    upData.bytes[i] = buff.data()[i];
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
   emit connectionTerminated("");
}

