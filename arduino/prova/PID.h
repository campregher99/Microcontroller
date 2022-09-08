#ifndef PID_H
#define PID_H

#include "Config.h"
#include "System.h"
#include "Function.h"

#define INPUT_ 3
#define OUTPUT_ 2

class PID
{
    System sys;
    float (*input)();
    void (*output)(float);
  public:
    PID();
    long unsigned int begin_(float (*_input)(), void (*_output)(float));
    void refresh();

  private:
    void setEEPROM();

};
#endif
