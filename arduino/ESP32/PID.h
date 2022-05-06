#ifndef PID_H
#define PID_H

#include "Controller.h"

#define EEPROM_SIZE sizeof(float)*5+sizeof(int)*3  //32 bytes

class PID : public Controller
{
    System pid;
public:
    PID(){};
    
    void begin(float* (*_input)(), void (*_output)(float*));
    void refresh();

private:
    void setEEPROM();
};
#endif