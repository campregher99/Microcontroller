#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "System.h"
#include "Arduino.h"
#include "Function.h"
#include "EEPROM.h"

#define SEPARATOR '/'
#define STARTER "?"
#define ENDER '!'
#define OK_MSG "?OK!"
#define ERR_MSG "?ER!"

class Controller
{
protected:
    float* (*input)();
    void (*output)(float*);
public:
    Controller(){};
    virtual void begin(float* (*_input)(), void (*_output)(float*));
    virtual void refresh();
};
#endif