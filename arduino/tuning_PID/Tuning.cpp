#include "Tuning.h"

void Tuning::begin_(float (*_input)(), void (*_output)(float))
{
  //initialize input/output functions
  input = _input;
  output = _output;

  // configure LED PWM functionalitites
  ledcSetup(CHANNEL_PWM, FREQ_PWM, RESOLUTION_PWM);

  // attach the channel to the GPIO to be controlled
  ledcAttachPin(PIN_PWM, CHANNEL_PWM);

  String* msg;
  //serial initialization and synchronization
  Serial.begin(115200);
  wait_serial();

#ifdef DEBUG
  Serial.println("inizio");
#endif

  Serial.println(OK_MSG);

  //read percentage of the output during the testing
  int len;
  msg = read_msg(&len);
  float perc = msg[0].toFloat();
#ifdef DEBUG
  Serial.println("percentuale");
  Serial.println(perc);
#endif

  Serial.println(OK_MSG);

  //variables initialization
  unsigned long long int time_delay, settling_time, sampling_time;
  float max_input, std, T, L, K, noise_;

  //measure noise
  noise_=noise(perc);

  //search delay time
  time_delay = search_delay(perc);

#ifdef DEBUG
  Serial.print("Delay time[s]:\t");
  Serial.println(time_delay / 1000000.0);
#endif

  //serch settling time
  settling_time = search_settling(perc, time_delay, &std, &max_input);

#ifdef DEBUG
  Serial.print("settling time found\nTs[s]:\t");
  Serial.println(settling_time / 1000000.0);
#endif

  sampling_time = settling_time / 10000;
  K = max_input / perc;		//dc gain of the process
  areas_mtd(perc, sampling_time, max_input, &L, &T);
  String send_;
  send_ = STARTER;
  send_ += SEPARATOR;
  Serial.print(send_);
  Serial.print(K, 9);
  Serial.print(SEPARATOR);
  Serial.print(L, 9);
  Serial.print(SEPARATOR);
  Serial.print(T, 9);
  Serial.print(SEPARATOR);
  Serial.print(std, 9);
  Serial.print(SEPARATOR);
  Serial.print(noise_, 9);
  Serial.println(ENDER);

#ifdef DEBUG
  Serial.print("Areas method result:\nK:\t");
  Serial.println(K);
  Serial.print("L:\t");
  Serial.println(L);
  Serial.print("T:\t");
  Serial.println(T);
  Serial.print("standard deviation:\t");
  Serial.println(std);
#endif
}

unsigned long long int Tuning::search_delay(float _perc)
{
  float initial_input;
  unsigned long long int time_;
  Queue inputs(SIZE_I);
  initial_input = input();
  ledcWrite(CHANNEL_PWM, _perc * (pow(2, RESOLUTION_PWM) - 1) / 100);
  time_ = micros();
  inputs.push(initial_input);
  while (initial_input >= inputs.mean())
  {
    inputs.push(input());
  }
  time_ = micros() - time_;
  ledcWrite(CHANNEL_PWM, 0);
  delayMicroseconds(time_ * 1.5);
  return time_;
}

unsigned long long int Tuning::search_settling(float _perc, unsigned long long int _delay, float* _std, float* _max_input)
{
  Queue inputs(SIZE_I);
  float initial_input, current_input, max_;
  unsigned long long int time_, initial_time, current_st = 0, old_st, sampling_time, delta_t;
  unsigned long int count, threshold;
  bool is_first = true;

  //searching minimum sampling time
  time_ = micros();
  inputs.push(input());
  max_ = inputs[0] - 1; //make sure that enter on the if statement to maximize the time
  current_input = inputs.mean();
  time_ += delta_t;
  if (max_ < current_input)
  {
    max_ = current_input;
    count = 0;
  } else {
    count++;
  }
#ifdef MONITOR
  Serial.println(max_);
#endif

  sampling_time = (micros() - time_) * 1.2;

  if (SIZE_I * sampling_time > _delay)
  {
    threshold = SIZE_I * 1.5;  //minimum value in order to be sure of full fill queue
  } else
  {
    threshold = _delay / sampling_time * 1.5; //minimum time to avoid the delay time
  }

  //searching settling time
  while (current_st / 1000.0 - old_st / 1000.0 > sampling_time * threshold / 1.3 * 1.1 / 1000.0 || is_first)
  {
    old_st = current_st;		//update settling time

    //setting up variables for acquisition
    inputs.clear_();
    initial_time = micros();      //acquisition initial time
    initial_input = input();      //acquisition initial sample
    ledcWrite(CHANNEL_PWM, _perc * (pow(2, RESOLUTION_PWM) - 1) / 100);  //output start up
    inputs.push(initial_input);
    time_ = initial_time;
    max_ = initial_input;       //setting up max value equal to first sample (for rising system output)
    count = 0;

#ifdef DEBUG
    Serial.print("initial input:\t");
    Serial.println(initial_input);
#endif

    //searching settling time with a specific threshold
    while (count < threshold)
    {
      delta_t = micros() - time_;
      if (delta_t >= sampling_time) {
        inputs.push(input());
        current_input = inputs.mean();
        time_ += delta_t;
        if (max_ < current_input)
        {
          max_ = current_input;
          count = 0;
        } else {
          count++;
        }
#ifdef MONITOR
        Serial.println(max_);
#endif
      }
    }
    current_st = micros() - initial_time;
    ledcWrite(CHANNEL_PWM, 0);  //reset the systems
#ifdef DEBUG
    Serial.print("settling time[s]:\t");
    Serial.println(current_st / 1000000.0);
    Serial.print("samppling time[us]:\t");
    Serial.println(sampling_time);
    Serial.print("max value reached:\t");
    Serial.println(max_);
    Serial.print("Threshold time[s]:\t");
    Serial.println(threshold * sampling_time / 1000000.0);
    Serial.print(current_st / 1000.0 - old_st / 1000.0);
    Serial.print(">");
    Serial.println(sampling_time * threshold / 1.3 * 1.1 / 1000.0);
#endif
    //waiting system reset
    delayMicroseconds(current_st * 1.5);
    while (input() > initial_input);
#ifdef DEBUG
    Serial.println("System resetted");
#endif
    *_std = inputs.std();
    *_max_input = max_;
#ifdef DEBUG
    Serial.print("final standard deviation:\t");
    Serial.println(*_std);
#endif
    is_first = false;
    threshold *= 1.3;
  }
  return current_st;
}

bool Tuning::areas_mtd(float _perc, unsigned long long int _sampling_time, float _max_input, float* _l, float* _t)
{
  Queue inputs(SIZE_I);
  float S1 = 0, S2 = 0, current_input;
  unsigned long long int time_, initial_time;
  inputs.push(input());
  ledcWrite(CHANNEL_PWM, _perc * (pow(2, RESOLUTION_PWM) - 1) / 100);
  time_ = micros();
  initial_time = time_;
  current_input = inputs.mean();
  S1 += (_max_input - inputs.mean()) * _sampling_time / 1000.0;
  S2 += current_input * _sampling_time / 1000.0;
  while (current_input < _max_input)
  {
    if (micros() - time_ >= _sampling_time)
    {
      time_ = micros();
      inputs.push(input());
      current_input = inputs.mean();
      S1 += (_max_input - current_input) * _sampling_time / 1000.0;
      if (current_input < _max_input * 0.632)
      {
        S2 += current_input * _sampling_time / 1000.0;
      }
    }
  }
  initial_time = micros() - initial_time;
  S1 /= 1000.0;
  S2 /= 1000.0;
#ifdef DEBUG
  Serial.print("settling time[s]:\t");
  Serial.println(initial_time / 1000000.0);
  Serial.print("sampling time[us]:\t");
  Serial.println(_sampling_time);
  Serial.print("S1:\t");
  Serial.println(S1);
  Serial.print("S2:\t");
  Serial.println(S2);
#endif

  *(_t) = E * S2 / _max_input;
  *(_l) = (S1 - _max_input * (*_t)) / _max_input;
  return true;
}

float Tuning::noise(float _perc)
{
	Queue inputs(SIZE_I*4);
	for(int i = 0; i < SIZE_I * 4;i++)
	{
		inputs.push(input());
	}
	return inputs.max_()/3.0;
}
