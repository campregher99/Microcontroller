#include "System.h"


void system_inizializer(System* _sys, unsigned short int _number_input, unsigned short int _number_output)
{
    queueInitializer(&(_sys->input), _number_input);
    queueInitializer(&(_sys->output), _number_output);
}


float sys_out(System* _sys, float _input)
{
    float out{0};
    pushQueue(&(_sys->input), _input);
    for(int i = 0; i < _sys->number_input; i++)
    {
        out += _sys->input.elements[i]*_sys->input_coeff[i];
    }
    for(int i = 0; i < _sys->number_output; i++)
    {
        out += _sys->output.elements[i]*_sys->output_coeff[i];
    }
    pushQueue(&(_sys->output),out);
    return out;
}


void sys_set_coeffs(System* _sys, float* _input_coeff, float* _output_coeff)
{
    _sys->input_coeff = _input_coeff;
    _sys->output_coeff = _output_coeff;
}


void sys_sys_set_coeffs(System* _sys, float* _input_coeff, float* _output_coeff, unsigned short int _number_input, unsigned short int _number_output)
{
    queueInitializer(&(_sys->input), _number_input);
    queueInitializer(&(_sys->output), _number_output);
    _sys->input_coeff = _input_coeff;
    _sys->output_coeff = _output_coeff;
}
