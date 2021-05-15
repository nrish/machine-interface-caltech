#ifndef SERIALMANAGER_H
#define SERIALMANAGER_H
#include <QtSerialPort>
#include <queue>
#include <stdint.h>

class SerialManager : public QObject
{
private:
    std::queue<uint8_t> log;
    QSerialPort conn;
    bool connected;
public:
    SerialManager();
    /**
     * @brief sendData writes data in buffer to serial port
     * @param buf buffer to write
     * @param size size of the buffer
     */
    void sendData(uint8_t* buf, int size);
    /**
     * @brief readData reads into buf from queue until amount is reached or no more data is available.
     * @param buf buffer to store bytes in
     * @param amount number of bytes to store
     */
    void getData(uint8_t* buf, int amount);
    void connectToPort(QSerialPortInfo port);
    int getBytesReady();
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
};
#endif // SERIALMANAGER_H
