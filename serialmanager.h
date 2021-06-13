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
    bool errorState;
    QMutex threadMutex;
    QQueue< QByteArray > commandQueue;
public:
    SerialManager();
    virtual ~SerialManager();
    int exec();
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

    void disconnect();

    void queueCommand(QByteArray data);

    void connectToPort(QSerialPortInfo port);

    void getSerialPort(QSerialPort& port) const;
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

    void connectionTerminated(QSerialPort::SerialPortError error);

    void connected();

};
#endif // SERIALMANAGER_H
