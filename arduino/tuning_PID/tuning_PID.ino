#include "Tuning.h"

float input();
void output(float _output);

Tuning t;
void setup() {
  // put your setup code here, to run once:
  t.begin_(input,output);
}

void loop() {
  // put your main code here, to run repeatedly:

}

float input()
{
  return (float)analogRead(4);
}

void output(float _output)
{
  ledcWrite(CHANNEL_PWM, _output);
}
