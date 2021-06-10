#ifndef SERIAL_DATA_H
#define SERIAL_DATA_H
#include <stdint.h>
/*
 * this file contains all the structures and unions required to easily serialize data and send it accross the serial port or write to EEPROM.
 * this file is taken from the machine control code.
 */

struct __attribute__((__packed__)) Tray{
  int32_t x; // index 0
  int32_t y; // index 1
};

struct __attribute__((__packed__)) StartData{
  uint8_t start_mode; //start mode can be 0 or 1 respectively normal run and calibration
  uint32_t mills; //ms/w
  uint8_t endWellIndex;
  uint8_t tray_index;
};

union StartDataSerialized{
  StartData startData;
  uint8_t bytes[sizeof(StartData)];
};


//EEPROM serializable data

struct __attribute__((__packed__)) CalibrationValues{
  int16_t WELL_DIST_X;
  int16_t WELL_DIST_Y;
  
  uint16_t TRAY_DIST_X;
  uint16_t TRAY_DIST_Y;

  bool Y_END_DIR;
  bool X_END_DIR;

  Tray trays[8];
};
union EEPROMData{
  uint8_t bytes[sizeof(CalibrationValues)];
  CalibrationValues values;
};


#endif
