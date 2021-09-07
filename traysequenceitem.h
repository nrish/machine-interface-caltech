#ifndef TRAYSEQUENCEITEM_H
#define TRAYSEQUENCEITEM_H

#include <QListWidgetItem>

class TraySequenceItem : public QListWidgetItem
{
private:
    int time;
    int startWell;
    int endWell;
    int index;
public:
    TraySequenceItem(int time, int startwell, int endwell, int index);
    int getTime() const;
    int getStartWell() const;
    int getEndWell() const;
    int getIndex() const;
};

#endif // TRAYSEQUENCEITEM_H
