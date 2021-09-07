#ifndef MAINWINDOW_H
#define MAINWINDOW_H
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

    void on_calibrateButton_clicked();

    void on_actionWell_Layout_triggered();

    void on_refreshButton_clicked();

    void on_calibration_recieve();

    void SerialConnectionTerminated(QString error);

    void SerialConnected();

    void on_tray_add_clicked();
    void on_tray_remove_clicked();

public slots:
    void newTraySequence(int time, int startwell, int endwell, int index);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
