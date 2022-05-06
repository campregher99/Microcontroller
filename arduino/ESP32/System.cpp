#include "System.h"


System::System(unsigned short int _number_input, unsigned short int _number_output)
{
    input.set(_number_input);
    output.set(_number_output);
}


float System::out(float _input)
{
    float out{0};
    input.push(_input);
    for(int i = 0; i < number_input; i++)
    {
        out += input[i]*input_coeff[i];
    }
    for(int i = 0; i < number_output; i++)
    {
        out += output[i]*output_coeff[i];
    }
    output.push(out);
    return out;
}


void System::set_coeffs(float* _input_coeff, float* _output_coeff)
{
    input_coeff = _input_coeff;
    output_coeff = _output_coeff;
}


void System::set_coeffs(float* _input_coeff, float* _output_coeff, unsigned short int _number_input, unsigned short int _number_output)
{
    input.set(_number_input);
    output.set(_number_output);
    input_coeff = _input_coeff;
    output_coeff = _output_coeff;
}