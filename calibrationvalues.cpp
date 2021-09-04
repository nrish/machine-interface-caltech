#include "calibrationvalues.h"

CalibrationValues::CalibrationValues()
{

}

tray &CalibrationValues::getTray(int index)
{
    if(index < 10 && index >= 0){
        return trays[index];
    }
    throw std::invalid_argument("tray index out of bounds");
}

void CalibrationValues::setTray(int index, uint16_t x, uint16_t y)
{
    if(index < 10 && index >= 0){
        trays[index].xpos = x;
        trays[index].ypos = y;
    }
    throw std::invalid_argument("tray index out of bounds");
}

void CalibrationValues::setWellDistX(uint16_t x)
{
    well_dist_x = x;
}

const uint16_t &CalibrationValues::getWellDistX()
{
    return well_dist_x;
}

void CalibrationValues::setWellDistY(uint16_t y)
{
    well_dist_y = y;
}

const uint16_t &CalibrationValues::getWellDistY()
{
    return well_dist_y;
}

bool CalibrationValues::getXAxisDir()
{
    return x_axis_dir;
}

void CalibrationValues::setXAxisDir(bool val)
{
    x_axis_dir = val;
}

bool CalibrationValues::getYAxisDir()
{
    return y_axis_dir;
}

void CalibrationValues::setYAxisDir(bool val)
{
    y_axis_dir = val;
}
