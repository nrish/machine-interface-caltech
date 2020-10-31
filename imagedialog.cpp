#include "imagedialog.h"
#include "ui_imagedialog.h"
#include <QIcon>

ImageDialog::ImageDialog(QWidget *parent, QString filePath, QString title) :
    QDialog(parent),
    ui(new Ui::ImageDialog)
{
    ui->setupUi(this);
    setWindowTitle(title);
    ui->imageLabel->setPixmap(QPixmap(filePath));
}

ImageDialog::~ImageDialog()
{
    delete ui;
}


