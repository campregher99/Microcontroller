
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
  // put your main code here, to run repeatedly:

}

float input()
{
  return (float)analogRead(4);
}

void output(float _output)
{
  ledcWrite(CHANNEL_PWM, _output * (pow(2, RESOLUTION_PWM) - 1)/100);
}
