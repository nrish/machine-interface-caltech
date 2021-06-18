#ifndef CALIBRATIONDIALOG_H
#define CALIBRATIONDIALOG_H
#include "serialData/serialData.h"
#include <QDialog>
#include "serialmanager.h"
namespace Ui {
class CalibrationDialog;
}

class CalibrationDialog : public QDialog
{
    Q_OBJECT
private:
    SerialManager* serialManager;
    CalibrationValueSerialized* calibration;
    bool doneLoading;
public:
    explicit CalibrationDialog(QWidget *parent, SerialManager* serialManager, CalibrationValueSerialized* calibration);
    ~CalibrationDialog();

private slots:
    void on_DialogButtons_accepted();

    void on_traySelect_currentIndexChanged(int index);

    void on_TrayX_valueChanged(int i);

    void on_TrayY_valueChanged(int i);

    void serialStatusUpdate();
    void on_testPos_clicked();

private:
    CalibrationValues values;
    Ui::CalibrationDialog *ui;
};

#endif // CALIBRATIONDIALOG_H
