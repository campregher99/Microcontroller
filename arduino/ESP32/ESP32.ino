#include "PID.h"

float* input();
void output(float* _output);

#define PIN 13
volatile bool is_timer_end;
PID sys;
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR interrrupt_timer()
{
  portENTER_CRITICAL_ISR(&timerMux);
  is_timer_end=true;
  portEXIT_CRITICAL_ISR(&timerMux);
}


void setup() {
  uint64_t T;
  pinMode(PIN, OUTPUT);
  T = begin_(&sys, input, output);
  timer = timerBegin(0, 80, true);

  timerAttachInterrupt(timer, &interrrupt_timer, true);
  timerAlarmWrite(timer, T, true);
  Serial.println(T);
  timerAlarmEnable(timer);
  Serial.println(T);
}

void loop() {
if(is_timer_end)
{
  is_timer_end=false;
  sys.refresh();
}
}



float* input()
{
  return new float {(float)analogRead(A0)};
}

void output(float* _output)
{
  Serial.println(_output[0]);
}
