#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Config.h"
#include "System.h"
#include "Function.h"


#define EEP_TS 180 //locatione where is saved sampling time in eeprom

class Controller
{
    System sys;
    float (*input)();
    void (*output)(float);
  public:
    Controller(){};
    uint64_t begin_(float (*_input)(), void (*_output)(float));
    void refresh();

  private:
    void setEEPROM();

};
#endif
