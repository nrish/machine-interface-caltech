#include "deviceManager.h"
#include "exception"
#include "string"
#include <mutex>
#include <QTime>

void DeviceManager::sendCommand(QByteArray arry)
{
    if(this->isReadySend){
        sock.write(arry);
        this->isReadySend = false;
    }else{
        this->pendingCommands.append(arry);
    }
}

DeviceManager::DeviceManager()
{
    connect(&this->sock, &QSerialPort::readyRead, this, &DeviceManager::dataRecieved);
    connect(&this->sock, &QSerialPort::errorOccurred, this, &DeviceManager::onError);
    this->isReadySend=true;
}
DeviceManager::~DeviceManager(){
}

void DeviceManager::printTestInfo()
{
    QByteArray data;
    data.append((char)0);
    data.append(id_printTestInfo);
    sendCommand(data);
}

void DeviceManager::setPos(uint16_t x, uint16_t y)
{
    x = qToLittleEndian(x);
    y = qToLittleEndian(y);
    QByteArray data;
    data.append(4);
    data.append(id_setPos);
    data.append((char*)&x, 2);
    data.append((char*)&y, 2);
    sendCommand(data);
}

void DeviceManager::home()
{
    QByteArray data;
    data.append((byte)0);
    data.append(id_home);
    sendCommand(data);
}

void DeviceManager::addTray(uint8_t index, uint16_t time, uint8_t start, uint8_t end)
{
    QByteArray data;
    data.append(5);
    data.append(id_addTray);
    data.append(index);
    time = qToLittleEndian(time);
    data.append((char*)&time, 2);
    data.append(start);
    data.append(end);
    qDebug() << "addTray size:" << data.size();
    qDebug() << data.toHex(' ');
    sendCommand(data);
}

void DeviceManager::removeTray(uint8_t index)
{

    QByteArray data;
    data.append(1);
    data.append(id_removeTray);
    data.append(index);
    sendCommand(data);
}

void DeviceManager::clearTrays()
{

    QByteArray data;
    data.append((char)0);
    data.append(id_clearTrays);
    sendCommand(data);
}

void DeviceManager::start()
{

    QByteArray data;
    data.append((char)0);
    data.append(id_start);
    sendCommand(data);
}

void DeviceManager::setDistCalibration(uint16_t WELL_DIST_X, uint16_t WELL_DIST_Y, bool X_END_DIR, bool Y_END_DIR)
{
    WELL_DIST_X = qToLittleEndian(WELL_DIST_X);
    WELL_DIST_Y = qToLittleEndian(WELL_DIST_Y);
    QByteArray data;
    data.append(6);
    data.append(id_setDistCalibration);
    data.append((char*)&WELL_DIST_X, 2);
    data.append((char*)&WELL_DIST_Y, 2);
    data.append(X_END_DIR);
    data.append(Y_END_DIR);
    sendCommand(data);
}

void DeviceManager::setTrayCalibration(uint8_t index, uint16_t x, uint16_t y)
{
    x = qToLittleEndian(x);
    y = qToLittleEndian(y);
    QByteArray data;
    data.append(5);
    data.append(id_setTrayCalibration);
    data.append(index);
    data.append((char*)&x, 2);
    data.append((char*)&y, 2);
    sendCommand(data);
}

void DeviceManager::confirmCalibration()
{
    QByteArray data;
    data.append((char)0);
    data.append(id_confirmCalibration);
    sendCommand(data);
}

void DeviceManager::getCalibrationData()
{
    QByteArray data;
    data.append((char)0);
    data.append(id_getCalibrationData);
    sendCommand(data);
}

QSerialPort &DeviceManager::getSerialPort()
{
    return this->sock;
}


CalibrationValues &DeviceManager::getCalibrationValues()
{
    return this->deviceCalibration;
}

void DeviceManager::connectToPort(QSerialPortInfo port)
{
    if(sock.isOpen())
        sock.close();
    sock.setPort(port);
    sock.setParity(QSerialPort::NoParity);
    sock.setStopBits(QSerialPort::OneStop);
    sock.setBaudRate(QSerialPort::Baud9600);
    sock.setDataBits(QSerialPort::Data8);
    if(sock.open(QIODevice::ReadWrite)){
        emit connected();
    }

}

void DeviceManager::dataRecieved(){
    qDebug() << "got bytes";
    qDebug() << QString(sock.peek(sock.bytesAvailable()).toHex());
    if(sock.bytesAvailable() >= 2){
        //there is data available
        uint16_t data;
        char* ptr = (char*)(&data);
        sock.peek(ptr, 2);
        unsigned char payloadSize = ptr[0];
        unsigned char functionID = ptr[1];
        if(sock.bytesAvailable() < payloadSize)
            return;
        sock.skip(2);
        qDebug() << "function ID #" << functionID << " payload size " << payloadSize;
        if(functionID == 0xFF){
            qDebug() << "Arduino ready for next command";
            this->isReadySend = true;
            if(!this->pendingCommands.isEmpty()){
                sock.write(this->pendingCommands.takeFirst());
                this->isReadySend = false;
            }
        }
        if(functionID == id_calibrationDataResponse){
            qDebug() << "got calibration data";
            //4 bytes for pos, 2 bytes for axis dirs, 8x4=32 for tray positions = 38
            sock.read((char*)(&deviceCalibration.well_dist_x), 2);
            sock.read((char*)&deviceCalibration.well_dist_y, 2);
            sock.read((char*)&deviceCalibration.x_axis_dir, 1);
            sock.read((char*)&deviceCalibration.y_axis_dir, 1);
            for(int i = 0; i < 8; i++){
                sock.read((char*)&deviceCalibration.getTray(i).xpos, 2);
                sock.read((char*)&deviceCalibration.getTray(i).ypos, 2);
            }
            qDebug() << "config: " << "well dist x " << deviceCalibration.well_dist_x << " well dist y " << deviceCalibration.well_dist_y
                     << " x axis dir " << deviceCalibration.x_axis_dir << "y axis dir" << deviceCalibration.y_axis_dir;
            for(int i = 0; i < 8; i++)
                qDebug() << "tray #" << i << " x:" << deviceCalibration.trays[i].xpos << " y: " << deviceCalibration.trays[i].ypos;
            qDebug() << "bytes left: " << sock.bytesAvailable();
            emit calibrationDataRecieved();
        }
    }
}

void DeviceManager::onError(QSerialPort::SerialPortError error){
    //?
    if(error == QSerialPort::SerialPortError::NoError){
        //shouldn't be here?
    } else {
        if(this->sock.isOpen()){
            emit connectionTerminated(sock.errorString());
            sock.close();
        }
    }
}

void DeviceManager::onAboutToClose()
{
    emit connectionTerminated("");
}

