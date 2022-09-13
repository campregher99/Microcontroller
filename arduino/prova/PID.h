#ifndef PID_H
#define PID_H

#include "Config.h"
#include "System.h"
#include "Function.h"

#define INPUT_ 3
#define OUTPUT_ 2
#define EEP_TS 24 //locatione where is saved sampling time in eeprom

class PID
{
    System sys;
    float (*input)();
    void (*output)(float);
  public:
    PID();
    uint64_t begin_(float (*_input)(), void (*_output)(float));
    void refresh();

  private:
    void setEEPROM();

};
#endif
