#include "System.h"

System::System(unsigned int _input, unsigned int _output)
{
    input_coeff=new Queue(_input);
    input=new Queue(_input);
    output_coeff=new Queue(_output);
    output=new Queue(_output);
}

float System::out(float _input)
{
    input->push(_input);
    float result;
    result=*input*(*input_coeff)+(*output)*(*output_coeff);
    output->push(result);
    return result;
}

void System::set_in_coeff(float* _coeff)
{
    input_coeff->set_elements(_coeff);
}

void System::set_out_coeff(float* _coeff)
{
    output_coeff->set_elements(_coeff);
}

