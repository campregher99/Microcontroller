
#include "Controller.h"

Controller pid;

volatile bool is_interrupt = false;
float input();
void output(float _output);
float reference();

//declaration timer variable
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

//timer interrupt service routine
void IRAM_ATTR interrrupt_timer()
{
  portENTER_CRITICAL_ISR(&timerMux);  //disable interrupt
  is_interrupt = true;
  portEXIT_CRITICAL_ISR(&timerMux);   //enable interrupt
}

void setup() {
  uint64_t T;

  // configure LED PWM functionalitites
  ledcSetup(CHANNEL_PWM, FREQ_PWM, RESOLUTION_PWM);

  // attach the channel to the GPIO to be controlled
  ledcAttachPin(PIN_PWM, CHANNEL_PWM);

  //configure Controller
  T = pid.begin_(input, output, reference);

#ifdef DEBUG
  Serial.print("Pid inizialization ended\nSample time[us]:\t");
#endif
  Serial.println(T);

#ifdef MONITOR
  wait_serial();
  Serial.println("ref,input,output");
#endif

  //timer initialization
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &interrrupt_timer, true);
  timerAlarmWrite(timer, T, true);

#ifdef DEBUG
  Serial.println("timer initialized");
#endif

  timerAlarmEnable(timer);
}

void loop() {
  if (is_interrupt)
  {
    is_interrupt = false;
    pid.refresh();
  }
}

float input()
{
  //Insert here the input code

}

void output(float _output)
{
  //Insert here the output code

}

float reference()
{
  //Insert here the reference code
  
}
