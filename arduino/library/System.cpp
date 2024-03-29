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
  input.push(_input);
#ifdef DEBUG
  Serial.println("\ninput:");
  input.print_();
  Serial.println("output:");
  output.print_();
#endif
  float result;
  result = input * input_coeff + output * output_coeff;
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

void System::initialize(unsigned int _input, unsigned int _output)
{
  input_coeff.initialize(_input);
  input.initialize(_input);
  output_coeff.initialize(_output);
  output.initialize(_output);
}

void System::print_()
{
  Serial.println("System printed:");
  Serial.println("Input coefficents:");
  input_coeff.print_();
  Serial.println("Output coefficents:");
  output_coeff.print_();

}

void System::set_output(float _output)
{
  output[0]=_output;
}
