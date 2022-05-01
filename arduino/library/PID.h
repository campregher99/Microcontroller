#ifndef PID_H
#define PID_H

#include "Controller.h"

class PID : public Controller
{

public:
    PID();
    ~PID();
    
    void begin();
};
#endif