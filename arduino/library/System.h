#ifndef SYSTEM_H
#define SYSTEM_H
#include "SetUp.h"
#include "Queue.h"

class System
{
    Queue input_coeff;
    Queue output_coeff;
    Queue input;
    Queue output;
  public:
    //Constructors
    System(unsigned int _input, unsigned int _output);
    System() {};

    float out(float _input);    //update the system status
    void set_in_coeff(float* _coeff);   //set input coefficents
    void set_out_coeff(float* _coeff);  //set output coefficents
    void set_coeffs(float* _c_in, float* _c_out);   //set all coefficents
    void initialize(unsigned int _input, unsigned int _output); //initialize the system for empty constructor
    void print_();      //print the coefficent on Serial
    void set_output(float _output);   //overwrite the last value of output
};

#endif
