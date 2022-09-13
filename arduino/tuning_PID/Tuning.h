#ifndef TUNING_H
#define TUNING_H 
#include "Config.h"
#include "Function.h"
#include "Queue.h"

#define SIZE_I 10
#define SIZE_D 10

class Tuning
{
	float (*input)();
    void (*output)(float);
public:
	Tuning(){};
	void begin_(float (*_input)(), void (*_output)(float));
};
#endif
