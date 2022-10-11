
#include <Estimating.h>

float input();
void output(float _output);

Estimating t;
void setup() {

  // configure LED PWM functionalitites
  ledcSetup(CHANNEL_PWM, FREQ_PWM, RESOLUTION_PWM);

  // attach the channel to the GPIO to be controlled
  ledcAttachPin(PIN_PWM, CHANNEL_PWM);

  t.begin_(input, output);
}

void loop() {

}

float input()
{
  //Insert here the input code
}

void output(float _output)
{
  //Insert here the output code
}
