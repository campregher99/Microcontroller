#ifndef SYSTEM_H
#define SYSTEM_H

#include "Queue.h"

class System
{
    Queue* input_coeff;
    Queue* output_coeff;
    Queue* input;
    Queue* output;
public:
    System(unsigned int _input, unsigned int _output);
    System(){};
    ~System();
    float out(float _input);
    void set_in_coeff(float* _coeff);
    void set_out_coeff(float* _coeff);
    void initialize(unsigned int _input, unsigned int _output)
};

#endif
