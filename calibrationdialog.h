#ifndef CALIBRATIONDIALOG_H
#define CALIBRATIONDIALOG_H
#include "calibrationvalues.h"
#include <QDialog>
#include "deviceManager.h"
namespace Ui {
class CalibrationDialog;
}

class CalibrationDialog : public QDialog
{
    Q_OBJECT
private:
    DeviceManager* deviceManager;
    bool doneLoading;
public:
    explicit CalibrationDialog(QWidget *parent, DeviceManager* serialManager);
    void refresh();
    ~CalibrationDialog();

private slots:
    void on_DialogButtons_accepted();

    void on_traySelect_currentIndexChanged(int index);

    void on_TrayX_valueChanged(int i);

    void on_TrayY_valueChanged(int i);

    void on_testPos_clicked();

    void on_homeButton_clicked();

private:
    CalibrationValues values;
    Ui::CalibrationDialog *ui;
};

#endif // CALIBRATIONDIALOG_H
