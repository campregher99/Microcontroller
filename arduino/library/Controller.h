#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "System.h"
#include "Arduino.h"

#define EEPROM_SIZE sizeof(float)*20+sizeof(int)*3  //92 bytes
#define SEPARATOR '/'
#define STARTER "?"
#define ENDER '!'

class Controller
{
public:
    Controller();
    ~Controller();
    virtual void begin();
    virtual 
};
#endif