#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "SetUp.h"
#include "System.h"
#include "Function.h"
#include <EEPROM.h>


#define EEP_TS 180 //locatione where is saved sampling time in eeprom
#define EEPROM_SIZE 188
#define NOT_INI_EEP "?NE!"
#define INI_EEP "?IE!"

class Controller
{
    System sys;
    float (*input)();
    void (*output)(float);
    float (*reference)();
    bool is_setup=true;
  public:
    Controller(){};
    uint64_t begin_(float (*_input)(), void (*_output)(float), float (*_reference)());
    void refresh();

  private:
    void setEEPROM();

};
#endif
