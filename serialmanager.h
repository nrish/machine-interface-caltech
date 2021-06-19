#ifndef SERIALMANAGER_H
#define SERIALMANAGER_H
#include <QtSerialPort>
#include <queue>
#include <stdint.h>
#include "serialData/serialData.h"
#include <mutex>

class SerialManager : public QObject
{
    Q_OBJECT
private:
    QSerialPort sock;
    QByteArray buff;
    bool connectionStatus;
    void readPacketStream(QByteArray &arry, size_t len);
    void writePacketStream(char *bytes, size_t len);
public:
    SerialManager();
    virtual ~SerialManager();
    /**
     * @brief sendRequest sends request for data
     * @param e expect, what you expect the arduino to send. note that request value will be ignored.
     */
    void sendRequest(expect e);
    /**
     * @brief sendCommand sends command to execute
     * @param e tells arduino what to expect, what command you are sending
     * @param bytes bytes of command
     */
    void sendCommand(expect e, uint8_t* bytes);
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

    void connectionTerminated(QString error);

    void connected();

};
#endif // SERIALMANAGER_H
