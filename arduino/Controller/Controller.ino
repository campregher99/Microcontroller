
#include "Controller.h"

Controller pid;

volatile bool is_interrupt = false;
float input();
void output(float _output);

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
  T = pid.begin_(input, output);

#ifdef DEBUG
  Serial.print("Pid inizialization ended\nSample time[us]:\t");
#endif
  Serial.println(T);

#ifdef MONITOR
  wait_serial();
  Serial.println("input,ref,output");
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
  static bool is_first = true;
  float input;
  static float ref = 0;
  input = (float)analogRead(4);
  if (is_first)
  {
    delayMicroseconds(500);
#ifdef MONITOR
    delayMicroseconds(280);
#endif
  } else
  {
#ifdef MONITOR
    Serial.print(input);
    Serial.print(",");
    Serial.print(ref);
    Serial.print(",");
#endif
  }
  if (Serial.available())
  {
    ref = Serial.readStringUntil(ENDER).toFloat();
    clear_in_buffer();
  }
  is_first = false;
  return ref - input;
}

void output(float _output)
{
  float out=_output;
#ifdef MONITOR
  static bool is_first = true;
  if (is_first)
  {
    delayMicroseconds(180);
  } else
  {
    Serial.println(_output);
  }
  is_first = false;
#endif
  ledcWrite(CHANNEL_PWM, _output * (pow(2, RESOLUTION_PWM) - 1) / 100);
}
