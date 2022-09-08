
#include "PID.h"
float input();
void output(float _output);


PID pid;
void setup() {
  pid.begin_(input, output);


}

void loop() {

}

float input()
{
  return (float)analogRead(A0);
}

void output(float _output)
{
  Serial.println(_output);
}
