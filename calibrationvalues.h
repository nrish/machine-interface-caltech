#ifndef CALIBRATIONVALUES_H
#define CALIBRATIONVALUES_H
#include <cstdint>
#include <stdexcept>
struct tray{
    uint16_t xpos;
    uint16_t ypos;
};

class CalibrationValues
{
public:
    tray trays[8];
    uint16_t well_dist_x;
    uint16_t well_dist_y;
    bool x_axis_dir;
    bool y_axis_dir;
    CalibrationValues();
    tray & getTray(int index);
    void setTray(int index, uint16_t x, uint16_t y);
    void setWellDistX(uint16_t);
    const uint16_t & getWellDistX();
    void setWellDistY(uint16_t);
    const uint16_t & getWellDistY();
    bool getXAxisDir();
    void setXAxisDir(bool val);
    bool getYAxisDir();
    void setYAxisDir(bool val);

};

#endif // CALIBRATIONVALUES_H
