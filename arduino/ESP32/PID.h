#ifndef PID_H
#define PID_H

#include "System.h"
#include "Arduino.h"
#include "Function.h"
#include "EEPROM.h"

#define SEPARATOR '/'
#define STARTER "?"
#define ENDER '!'
#define OK_MSG "?OK!"
#define ERR_MSG "?ER!"
#define EEPROM_SIZE sizeof(float)*5+sizeof(int)*3  //32 bytes

class PID
{
public:
    PID();
    ~PID();
    
};
struct PID
{
    System pid;
    float* (*_input)();
    void (*_output)(float*);
};

    
    long unsigned int begin_(PID* _pid, float* (*_input)(), void (*_output)(float*));
    void refresh(PID* _pid);


    void setEEPROM();
    String* read_msg(int* _len);

#endif
