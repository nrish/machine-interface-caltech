#ifndef UINT32SPINBOX_H
#define UINT32SPINBOX_H

#include <QAbstractSpinBox>

class uint32SpinBox : public QAbstractSpinBox
{
public:
    uint32SpinBox(QWidget *parent);
    void stepBy(int steps) override;
    StepEnabled  stepEnabled() const override;
    QValidator::State validate(QString &input, int &pos) const override;
    void fixup(QString &input) const override;
    uint32_t value() const;
    uint32_t minimum() {return 0;};
    uint32_t maximum() {return UINT32_MAX;};
private:
    bool isInRange(int steps) const;
    void updateText();
    uint32_t val;
    QRegExpValidator *validator;
};

#endif // UINT32SPINBOX_H
