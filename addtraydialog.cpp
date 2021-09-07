#include "addtraydialog.h"
#include "ui_addtraydialog.h"

addTrayDialog::addTrayDialog(MainWindow* window) :
    QDialog(window),
    ui(new Ui::addTray)
{
    this->mainWindow = window;
    ui->setupUi(this);
    ui->endSpinbox->setRange(0, 96);
    ui->endSpinbox->setValue(96);
    ui->startSpinbox->setRange(0, 96);
    ui->timeSpinbox->setRange(0, INT_MAX);

    for(int i = 0; i < 8; i++){
        ui->comboBox->addItem(QString::asprintf("Tray #%i", i));
    }
    connect(this, &addTrayDialog::newTraySequence, this->mainWindow, &MainWindow::newTraySequence);
}

addTrayDialog::~addTrayDialog()
{
    delete ui;
}

void addTrayDialog::on_confirm_accepted()
{
    emit newTraySequence(ui->timeSpinbox->value(), ui->startSpinbox->value(), ui->endSpinbox->value(), ui->comboBox->currentIndex());
}

