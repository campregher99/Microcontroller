#ifndef SYSTEM_H
#define SYSTEM_H

#include "Arduino.h"
#include "Queue.h"

struct System
{
    float* input_coeff;
    float* output_coeff;
    Queues input;
    Queues output;
    unsigned short int number_input;
    unsigned short int number_output;
};

    
    void system_inizializer(System* _sys,unsigned short int _number_input, unsigned short int _number_output);
    float sys_out(System* _sys, float _input);
    void sys_set_coeffs(System* _sys, float* _input_coeff, float* _output_coeff);
    void sys_set_coeffs(System* _sys, float* _input_coeff, float* _output_coeff,unsigned short int _number_input, unsigned short int _number_output);
    

    



#endif
