#include "System.h"

System::System(unsigned int _input, unsigned int _output)
{
    input_coeff.initialize(_input);
    input.initialize(_input);
    output_coeff.initialize(_output);
    output.initialize(_output);
}

float System::out(float _input)
{
    #ifdef DEBUG
    Serial.println("\ninput:");
    input.print_();
    Serial.println("output:");
    output.print_();
    #endif
    input.push(_input);
    float result;
    result=input*input_coeff+output*output_coeff;
    output.push(result);
    return result;
}

void System::set_in_coeff(float* _coeff)
{
    input_coeff.set_elements(_coeff);
}

void System::set_out_coeff(float* _coeff)
{
    output_coeff.set_elements(_coeff);
}

void System::set_coeffs(float* _c_in, float* _c_out)
{
    input_coeff.set_elements(_c_in);
    output_coeff.set_elements(_c_out);
}

void print_()
{
    Serial.println("Input coefficents:");
    input_coeff.print_();
    Serial.println("Output coefficents:");
    output_coeff.print_();
    
}
