#ifndef ADDTRAYDIALOG_H
#define ADDTRAYDIALOG_H

#include <QDialog>
#include "mainwindow.h"
namespace Ui {
class addTray;
}

class addTrayDialog : public QDialog
{
    Q_OBJECT

public:
    explicit addTrayDialog(MainWindow* window);
    ~addTrayDialog();
private slots:
    void on_confirm_accepted();
signals:
    void newTraySequence(int time, int startwell, int endwell, int index);
private:
    MainWindow *mainWindow;
    Ui::addTray *ui;
};

#endif // ADDTRAYDIALOG_H
