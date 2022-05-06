#include "PID.h"

float* input();
void output(float* _output);

#define PIN 13

PID sys;
void setup() {
	pinMode(PIN, OUTPUT);
	sys.begin(input,output);
	
	
}

void loop() {

}

float* input()
{
	return new float {(float)analogRead(A0)};
}

void output(float* _output)
{
	analogWrite(PIN, _output[0]);
}