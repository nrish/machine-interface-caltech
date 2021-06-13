#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "serialData/serialData.h"
#include <QMainWindow>
#include <QSerialPort>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_connectButton_clicked();

    void on_startButton_clicked();

    void on_stopButton_clicked();

    void on_actionTrayWell_triggered(bool);

    void on_refreshButton_clicked();

    void calibrationDataUpdate(CalibrationValueSerialized data);

    void updateDataRecieved(updateDataSerialized data);

    void SerialConnectionTerminated(QSerialPort::SerialPortError error);

    void SerialConnected();

    void on_calibrateButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
