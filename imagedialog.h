#ifndef IMAGEDIALOG_H
#define IMAGEDIALOG_H

#include <QDialog>

namespace Ui {
class ImageDialog;
}

class ImageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ImageDialog(QWidget *parent = nullptr, QString filePath = "", QString title = "Dialog");
    ~ImageDialog();

private:
    Ui::ImageDialog *ui;
};

#endif // IMAGEDIALOG_H
