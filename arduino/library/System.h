#ifndef SYSTEM_H
#define SYSTEM_H value

#include "Queue.h"

class System
{
    float* input_coeff;
    float* output_coeff;
    Queue* input;
    Queue* output;
    unsigned short int number_input;
    unsigned short int number_output;

public:
    System();
    System(unsigned short int _number_input, unsigned short int _number_output);
    ~System();
    float out(float _input);
    void set_coeffs(float* _input_coeff, float* _output_coeff);
    void set_coeffs(float* _input_coeff, float* _output_coeff,unsigned short int _number_input, unsigned short int _number_output);

    
};
#endif

