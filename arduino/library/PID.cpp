#include "PID.h"

void PID::begin(float* (*_input)(), void (*_output)(float*))
{
    input=_input;
    output=_output;
    Serial.begin(115200);
    EEPROM.begin(EEPROM_SIZE);
    int num_in, num_out,index;
    EEPROM.get(0,index);
    if(index!=32)
    {
        String* strs;
        String msg;
        int len;
        bool is_first=true;
        do
        {
            do
            {
                if(!is_first)
                {
                    Serial.print(ERR_MSG);
                }
                is_first=false;
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
        EEPROM.put(sizeof(int),num_in);
        EEPROM.put(sizeof(int)*2,num_out);
        for(int i = sizeof(int)*3; i - sizeof(int)*3 < sizeof(float) * num_in; i = i + sizeof(float))
        {
            EEPROM.put(i,input_coeff[(i-sizeof(int))/sizeof(float)]);
        }
        for(int i = sizeof(int)*3 + sizeof(float) * num_in; i - sizeof(int)*3 - sizeof(float) * num_in < sizeof(float) * num_out; i = i + sizeof(float))
        {
            EEPROM.put(i,output_coeff[(i-sizeof(int) - sizeof(float) * num_in)/sizeof(float)]);
        }
        pid.set_coeffs(input_coeff,output_coeff,num_in,num_out);
        long long unsigned int time;
        time=micros();
        refresh();
        time=micros()-time;
        Serial.print(STARTER);
        Serial.print(time);
        Serial.println(ENDER);
        Serial.println(OK_MSG);
    }
    else
    {
        EEPROM.get(sizeof(int),num_in);
        EEPROM.get(sizeof(int)*2,num_out);
        float* input_coeff = new float[num_in];
        float* output_coeff = new float[num_out];
        for(int i = sizeof(int)*3; i - sizeof(int)*3 < sizeof(float) * num_in; i = i + sizeof(float))
        {
            EEPROM.get(i,input_coeff[i]);
        }
        for(int i = sizeof(int)*3 + sizeof(float) * num_in; i - sizeof(int)*3 - sizeof(float) * num_in < sizeof(float) * num_out; i = i + sizeof(float))
        {
            EEPROM.get(i,output_coeff[i]);
        }
    }
}


void PID::refresh()
{
    float in, out;
    in=input()[0];
    out=pid.out(in);
    output(&out);
}
