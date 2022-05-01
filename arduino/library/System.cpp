#include "System.h"


System::System(unsigned short int _number_input, unsigned short int _number_output)
{
    input = new Queue<float>(_number_input);
    output = new Queue<float>(_number_output);
}


float System::out(float _input)
{
    float out{0};
    input.push(_input);
    for(int i = 0; i < number_input; i++)
    {
        out += (*input)[i]*input_coeff[i];
    }
    for(int i = 0; i < number_output; i++)
    {
        out += (*output)[i]*output_coeff[i];
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
    delete input;
    delete output;
    input = new Queue(_number_input);
    output = new Queue(_number_output);
    input_coeff = _input_coeff;
    output_coeff = _output_coeff;
}


void System::begin()
{
    Serial.begin(115200);
    EEPROM.begin(EEPROM_SIZE);
    int num_in, num_out;
    if(EEPROM.get(0,int)<0)
    {
        String* strs;
        String msg;
        int len;
        do
        {
            do
            {
                while(!Serial.available());
                msg = Serial.readStringUntil(ENDER);
                strs=split(msg,SEPARATOR,&len); 
            }while(strs[0]!=STARTER);
            num_in=strs[1].toInt();
            num_out=strs[2].toInt();
        }while(len!=num_in+num_out+3);
        float* input_coeff = new float[num_in];
        float* output_coeff = new float[num_out];
        for(int i = 3; i - 3 < num_in; i++)
        {
            input_coeff[i-3]=strs[i].toFloat();
        }
        for(int i = 3 + num_in; i - 3 - num_in < num_out; i++)
        {
            input_coeff[i - 3 - num_in]=strs[i].toFloat();
        }
        EEPROM.put(0,num_in+num_out+1);
        for(int i = sizeof(int); i - sizeof(int) < sizeof(float) * num_in; i = i + sizeof(float))
        {
            EEPROM.put(i,input_coeff[(i-sizeof(int))/sizeof(float)]);
        }
        for(int i = sizeof(int) + sizeof(float) * num_in; i - sizeof(int) - sizeof(float) * num_in < sizeof(float) * num_out; i = i + sizeof(float))
        {
            EEPROM.put(i,output_coeff[(i-sizeof(int) - sizeof(float) * num_in)/sizeof(float)]);
        }
        sys.set_coeffs(input_coeff,output_coeff,num_in,num_out);
    }
    else
    {

    }
}