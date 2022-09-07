#ifndef PID_H
#define PID_H 

#include "Config.h"
class PID
{
	System sys;
public:
	PID();
	long unsigned int begin_();

private:
	void setEEPROM();
	
};
#endif