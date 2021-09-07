#include "traysequenceitem.h"

TraySequenceItem::TraySequenceItem(int time, int startwell, int endwell, int index)
{
    this->time = time;
    this->startWell = startwell;
    this->endWell = endwell;
    this->index = index;
    this->setText(QString::asprintf("Tray #%i, wells %i to %i @ %i ms each", index+1, startWell, endWell, time));
}

int TraySequenceItem::getTime() const
{
    return time;
}

int TraySequenceItem::getStartWell() const
{
    return startWell;
}

int TraySequenceItem::getEndWell() const
{
    return endWell;
}

int TraySequenceItem::getIndex() const
{
    return index;
}

