#ifndef SERIALMANAGER_H
#define SERIALMANAGER_H
#include <QtSerialPort>
#include <queue>
#include <stdint.h>
#include "serialData/serialData.h"
#include <mutex>

class SerialManager : public QThread
{
    Q_OBJECT
private:
    QSerialPort sock;
    QByteArray buff;
    SerialManager(QSerialPortInfo port);
    void run();
    virtual ~SerialManager();
public:
    void disconnect();
    bool serialConnected();
    QSerialPort& getSerialPort();

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
    /**
     * @brief sends data over serial
     */
    void sendData(QByteArray data);

    void connected();
signals:
    /**
     * @brief calibrationDataRecieved got calibration data form arduino
     * @param data
     */
    void calibrationDataRecieved(CalibrationValueSerialized data);
    /**
     * @brief updateDataRecieved arduino updated it status
     * @param data
     */
    void updateDataRecieved(updateDataSerialized data);

    void connectionTerminated();
};
#endif // SERIALMANAGER_H
