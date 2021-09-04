#include "deviceManager.h"
#include "exception"
#include "string"
#include <mutex>
#include <QTime>

DeviceManager::DeviceManager()
{
    connect(&this->sock, &QSerialPort::readyRead, this, &DeviceManager::dataRecieved);
    connect(&this->sock, &QSerialPort::errorOccurred, this, &DeviceManager::onError);
}
DeviceManager::~DeviceManager(){
    this->connectionStatus = false;
}

void DeviceManager::printTestInfo()
{
    QByteArray data;
    data.append((char)0);
    data.append(id_printTestInfo);
    sock.write(data);
}

void DeviceManager::setPos(uint16_t x, uint16_t y)
{
    QByteArray data;
    data.append(4);
    data.append(id_setPos);
    data.append(qToLittleEndian(x));
    data.append(qToLittleEndian(y));
    sock.write(data);
}

void DeviceManager::home()
{
    QByteArray data;
    data.append((char)0);
    data.append(id_home);
    sock.write(data);
}
//windows pass 1600
void DeviceManager::addTray(uint8_t index, uint16_t time, uint8_t start, uint8_t end)
{
    QByteArray data;
    data.append(5);
    data.append(id_addTray);

    data.append(index);
    data.append(qToLittleEndian(time));
    data.append(start);
    data.append(end);
    sock.write(data);
}

void DeviceManager::removeTray(uint8_t index)
{
    QByteArray data;
    data.append(1);
    data.append(id_removeTray);
    data.append(index);
    sock.write(data);
}

void DeviceManager::clearTrays()
{
    QByteArray data;
    data.append((char)0);
    data.append(id_clearTrays);
    sock.write(data);
}

void DeviceManager::start()
{
    QByteArray data;
    data.append((char)0);
    data.append(id_start);
    sock.write(data);
}

void DeviceManager::setDistCalibration(uint16_t WELL_DIST_X, uint16_t WELL_DIST_Y, bool X_END_DIR, bool Y_END_DIR)
{
    QByteArray data;
    data.append(6);
    data.append(id_setDistCalibration);
    data.append(qToLittleEndian(WELL_DIST_X));
    data.append(qToLittleEndian(WELL_DIST_Y));
    data.append(X_END_DIR);
    data.append(Y_END_DIR);
    sock.write(data);
}

void DeviceManager::setTrayCalibration(uint8_t index, uint16_t x, uint16_t y)
{
    QByteArray data;
    data.append(5);
    data.append(id_setTrayCalibration);
    data.append(index);
    data.append(qToLittleEndian(x));
    data.append(qToLittleEndian(y));
}

void DeviceManager::confirmCalibration()
{
    QByteArray data;
    data.append((char)0);
    data.append(id_confirmCalibration);
    sock.write(data);
}

void DeviceManager::getCalibrationData()
{
    QByteArray data;
    data.append((char)0);
    data.append(id_getCalibrationData);
    sock.write(data);
}

CalibrationValues &DeviceManager::getCalibrationValues()
{
    return this->deviceCalibration;
}

void DeviceManager::connectToPort(QSerialPortInfo port)
{
    if(this->connectionStatus){
        this->sock.close();
    }
    sock.setPort(port);
    sock.setParity(QSerialPort::NoParity);
    sock.setStopBits(QSerialPort::OneStop);
    sock.setFlowControl(QSerialPort::NoFlowControl);
    sock.setBaudRate(QSerialPort::Baud9600);
    sock.setDataBits(QSerialPort::Data8);
    if(sock.open(QIODevice::ReadWrite)){
        emit connected();
    }

}

void DeviceManager::dataRecieved(){
    qDebug() << "got bytes";
    if(sock.bytesAvailable() >= 2){
        //there is data available
        char functionID;
        char payloadSize;
        sock.peek(&payloadSize, 1);
        sock.read(&functionID, 1);
        if(functionID == id_calibrationDataResponse){
            qDebug() << "got calibration data";
            //4 bytes for pos, 2 bytes for axis dirs, 8x4=32 for tray positions = 38
            sock.read((char*)&deviceCalibration.well_dist_x, 2);
            sock.read((char*)&deviceCalibration.well_dist_y, 2);
            sock.read((char*)&deviceCalibration.x_axis_dir, 1);
            sock.read((char*)&deviceCalibration.y_axis_dir, 1);
            for(int i = 0; i < 8; i++){
                sock.read((char*)&deviceCalibration.getTray(i).xpos, 2);
                sock.read((char*)&deviceCalibration.getTray(i).ypos, 2);
            }
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
            sock.close();
        }
        emit connectionTerminated(sock.errorString());
    }
}

void DeviceManager::onAboutToClose()
{
    emit connectionTerminated("");
}

