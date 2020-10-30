#include "uint32spinbox.h"
#include "qlineedit.h"
#include <QDebug>

uint32SpinBox::uint32SpinBox(QWidget *parent): QAbstractSpinBox(parent)
{
    QRegExp rx("\\d+");
    this->validator = new QRegExpValidator(rx);

    val = 2000;
    updateText();
}

bool uint32SpinBox::isInRange(int steps) const
{
    long result1 = this->val + steps;
    long result2 = this->val + steps;
    return result1 <= UINT32_MAX && result2 >= 0;
}

void uint32SpinBox::updateText()
{
    lineEdit()->setText(QString::fromStdString(std::to_string(val)));
}

void uint32SpinBox::stepBy(int steps)
{
    if(isInRange(steps)) val += steps;
    updateText();
}

QAbstractSpinBox::StepEnabled uint32SpinBox::stepEnabled() const
{
    bool up = this->isInRange(1);
    bool down = this->isInRange(-1);
    if(up && down){
        return QAbstractSpinBox::StepUpEnabled | QAbstractSpinBox::StepDownEnabled;
    }else if(up && !down){
        return QAbstractSpinBox::StepUpEnabled;
    }else if(!up && down){
        return QAbstractSpinBox::StepDownEnabled;
    }else{
        return QAbstractSpinBox::StepNone;
    }
}

QValidator::State uint32SpinBox::validate(QString &input, int &pos) const
{
    qDebug() << input;
    if(validator->validate(input, pos) != QValidator::Invalid) {
        if(input == "") return QValidator::Intermediate;

        QStringRef leastValueStr = QStringRef(&input, 0, pos);
        long value = std::stol(leastValueStr.toString().toStdString());
        long difference = UINT32_MAX - value;

        qDebug() << "difference: " << difference;

        if(difference < 0) {
            return QValidator::Invalid;
        }else if(difference >= 0){
            return QValidator::Acceptable;
        }
    }
    return QValidator::Invalid;
}

void uint32SpinBox::fixup(QString &input) const
{
    // We are not going to try to correct this really...
    input = tr("2000");
}

uint32_t uint32SpinBox::value() const
{
    return this->val;
}
