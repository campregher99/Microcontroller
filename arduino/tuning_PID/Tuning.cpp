#include "Tuning.h"

void Tuning::begin_(float (*_input)(), void (*_output)(float))
{
	//initialize input/output functions
  	input = _input;
  	output = _output;
  	Queue inputs(SIZE_I), derivat(SIZE_D);

	// configure LED PWM functionalitites
	ledcSetup(CHANNEL_PWM, FREQ_PWM, RESOLUTION_PWM);

	// attach the channel to the GPIO to be controlled
	ledcAttachPin(PIN_PWM, CHANNEL_PWM);

	String* msg;
	Serial.begin(115200);
	wait_serial();
  #ifdef DEBUG
  Serial.println("inizio");
  #endif
  int len;
	msg=read_msg(&len);
	float perc=msg[0].toFloat();
  #ifdef DEBUG
  Serial.println("percentuale");
  Serial.println(perc);
  #endif
	float initial_input, current_input, derivative, old_input;
	unsigned long long int time, delta_t;
	time=micros();
	initial_input=input();
	ledcWrite(CHANNEL_PWM, perc*(pow(2,RESOLUTION_PWM)-1)/100);
	inputs.push(initial_input);
	old_input=initial_input;
	while(true)
	{
		delta_t=micros()-time;
		for(int i = 0; i < SIZE_I; i ++)
		{
			inputs.push(input());
			delayMicroseconds(50);
		}
		current_input=inputs.mean();
		time+=delta_t;
		derivative=(current_input - old_input)/delta_t*1000;
		derivat.push(derivative);

    old_input=current_input;
    Serial.println(derivat.iae());
	}
}
