#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H
#include <QtSerialPort>
#include <queue>
#include <stdint.h>
#include <mutex>
#include "calibrationvalues.h"
typedef unsigned char byte;

const char id_printTestInfo = 0;
const char id_setPos = 1;
const char id_home = 2;
const char id_addTray = 3;
const char id_removeTray = 4;
const char id_clearTrays = 5;
const char id_start = 6;
const char id_setDistCalibration = 7;
const char id_setTrayCalibration = 8;
const char id_confirmCalibration = 9;
const char id_getCalibrationData = 10;

const unsigned char id_calibrationDataResponse = 51;

class DeviceManager : public QObject
{
    Q_OBJECT
private:
    QSerialPort sock;
    QByteArray buff;
    CalibrationValues deviceCalibration;
    QList<QByteArray> pendingCommands;
    void sendCommand(QByteArray arry);
    bool isReadySend = true;
public:
    DeviceManager();
    virtual ~DeviceManager();

    void printTestInfo();

    void setPos(uint16_t x, uint16_t y);

    void home();

    void addTray(uint8_t index, uint16_t time, uint8_t start, uint8_t end);

    void removeTray(uint8_t index);

    void clearTrays();

    void start();

    void setDistCalibration(uint16_t WELL_DIST_X, uint16_t WELL_DIST_Y, bool X_END_DIR, bool Y_END_DIR);

    void setTrayCalibration(uint8_t index, uint16_t x, uint16_t y);

    void confirmCalibration();

    void getCalibrationData();

    QSerialPort& getSerialPort();

    CalibrationValues& getCalibrationValues();
    /**
     * @brief disconnect from serial port
     */
    void disconnectSerial();
public slots:
    /**
     * @brief dataRecieved slot triggered when data is received from port
     * @param set
     */
    void dataRecieved();
    /**
     * @brief errorOccurred triggered when error occurs
     */
    void onError(QSerialPort::SerialPortError);
    /**
     * @brief onAboutToClose called when port is going to close
     */
    void onAboutToClose();

    void connectToPort(QSerialPortInfo port);

signals:
    void calibrationDataRecieved();

    void connectionTerminated(QString error);

    void connected();

};
#endif // DEVICEMANAGER_H
