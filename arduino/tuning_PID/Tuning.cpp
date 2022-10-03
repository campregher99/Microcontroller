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

  msg = read_msg(&len);

  if (msg[0] == "AM")
  {
    //variables initialization
    unsigned long long int time_delay, settling_time, sampling_time;
    float max_input, std, T, L, K, noise_;

    //measure noise
    noise_ = noise(perc);

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

    sampling_time = settling_time / SAMPLING_DIV;
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
  } else if (msg[0] == "ST")		//stream data for next analysis
  {
#ifdef DEBUG
    Serial.println("Streaming data");
#endif

    unsigned long long int time_delay, settling_time, sampling_time, time_;
    float max_input, std, noise_;

    //measure noise
    noise_ = noise(perc);

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

    sampling_time = settling_time / SAMPLING_DIV;

    Serial.print(STARTER);
    Serial.print(sampling_time, 9);
    Serial.println(ENDER);

    msg = read_msg(&len);
    if (msg[0] != "OK")
    {
      Serial.println(ERR_MSG);
      return;
    }

    time_ = micros();
    Serial.println(input());
    for (int i = 0; i < SAMPLING_DIV * 0.5 - 1; i++)
    {
      while (micros() - time_ < sampling_time);
      time_ = micros();
      Serial.println(input());
    }

    ledcWrite(CHANNEL_PWM, perc * (pow(2, RESOLUTION_PWM) - 1) / 100);

    for (int i = 0; i < SAMPLING_DIV * 1.5; i++)
    {
      while (micros() - time_ < sampling_time);
      time_ = micros();
      Serial.println(input());
    }

    Serial.println(OK_MSG);

  } else if (msg[0] == "RM")    //relay method
  {
    //reading delta percentage and maximum derivative of the controller output
    float d_perc, max_deriv, t_ult, k_ult, K, L, T, omega_ult;
    Serial.println(OK_MSG);

#ifdef DEBUG
    Serial.println("Send ?/max_deriv!");
#endif

    msg = read_msg(&len);
    max_deriv = msg[0].toFloat();

#ifdef DEBUG
    Serial.print("Delta percentage:\t");
    Serial.println(d_perc, 9);
    Serial.print("Max derivative[percentage/s]:\t");
    Serial.println(max_deriv, 9);
#endif

    relay_mtd(perc, max_deriv,  & K, & T, & L);

    String send_;
    send_ = STARTER;
    send_ += SEPARATOR;
    Serial.print(send_);
    Serial.print(K, 9);
    Serial.print(SEPARATOR);
    Serial.print(L, 9);
    Serial.print(SEPARATOR);
    Serial.print(T, 9);
    Serial.println(ENDER);

#ifdef DEBUG
    Serial.print("Relay method result:\nK:\t");
    Serial.println(K);
    Serial.print("L:\t");
    Serial.println(L);
    Serial.print("T:\t");
    Serial.println(T);
#endif
  }
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
    //while (input() > initial_input);
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
  Queue inputs(SIZE_I * 4);
  for (int i = 0; i < SIZE_I * 4; i++)
  {
    inputs.push(input());
  }
  return inputs.max_() / 3.0;
}

bool Tuning::relay_mtd(float _perc, float _max_deriv, float* _k, float* _t, float* _l)
{
  float rise_time = _perc / _max_deriv, act_perc = 0, max_input, min_input, noise, steady_input, input_av, hys, d_perc;
  Queue ult_times(10), inputs(SIZE_I), amplitude(10);
  unsigned long long int time_, d_time, ult_t;
  unsigned long int count = 0;
  bool is_high, is_steady_state = false;

  d_time = rise_time / (_perc * 10) * 1000000.0;

#ifdef DEBUG
  Serial.print("rise time[s]:\t");
  Serial.println(rise_time, 9);
  Serial.print("Frequency output variation[Hz]:\t");
  Serial.println(1 / rise_time * (_perc * 10), 9);
#endif

  //bringing the system at the steady state
  time_ = micros();
  ledcWrite(CHANNEL_PWM, act_perc );
  act_perc += 0.1;
  while (act_perc <= _perc)
  {
    if (micros() - time_ >= d_time)
    {
      time_ = micros();
      ledcWrite(CHANNEL_PWM, act_perc * (pow(2, RESOLUTION_PWM) - 1) / 100);
      act_perc += 0.1;
    }
  }

  //waiting for the steady state
  time_ = micros();
  inputs.push(input());
  max_input < inputs.max_();
  max_input = inputs.max_();
  count = 0;
  d_time = (micros() - time_) * 1.3;
  max_input = input();
  time_ = micros();
  while (count <= SIZE_I * 4)
  {
    if (micros() - time_ >= d_time)
    {
      time_ = micros();
      inputs.push(input());
      if (max_input < inputs.max_())
      {
        max_input = inputs.max_();
        count = 0;
      } else
      {
        count++;
      }
    }
  }

#ifdef DEBUG
  Serial.println("System at the steady state");
#endif

  //detection noise which will be the hysteresis
  for (int i = 0; i < SIZE_I; i++)
  {
    inputs.push(input());
    delayMicroseconds(SIZE_I);
  }
  noise = inputs.std() * 3;
  hys = noise * 5;
  steady_input = inputs.mean();
  *_k = steady_input / _perc;

  d_perc = hys * 2 / * _k;

#ifdef DEBUG
  Serial.print("noise (99%):\t");
  Serial.println(noise, 9);
  Serial.print("steady state input:\t");
  Serial.println(steady_input);
  Serial.print("K:\t");
  Serial.println(*_k);
  Serial.print("delta percentage:\t");
  Serial.println(d_perc);
#endif

  //start relay method
  inputs.clear_(steady_input);
  time_ = micros();
  ledcWrite(CHANNEL_PWM, (_perc + d_perc) * (pow(2, RESOLUTION_PWM) - 1) / 100);
  is_high = true;
  while (inputs.mean() < steady_input + noise) {
    inputs.push(input());
  }
  *_l = micros() - time_;
  while (!is_steady_state)
  {
    inputs.push(input());
    input_av = inputs.mean();

#ifdef MONITOR
    Serial.print(input_av);
    Serial.print(",");
    Serial.print(steady_input + hys);
    Serial.print(",");
    Serial.print(steady_input - hys);
    Serial.print(",");
    Serial.println(((is_high) ? _perc + d_perc : _perc - d_perc) * *_k);
#endif

    if (is_high)
    {
      if (input_av > steady_input + hys)
      {
        ledcWrite(CHANNEL_PWM, (_perc - d_perc) * (pow(2, RESOLUTION_PWM) - 1) / 100);
        is_high = false;
      }
      if (max_input < input_av)
        max_input = input_av;
    } else
    {
      if (input_av < steady_input - hys)
      {
        ult_t = micros() - time_;
        ledcWrite(CHANNEL_PWM, (_perc + d_perc) * (pow(2, RESOLUTION_PWM) - 1) / 100);
        time_ += ult_t;
        ult_times.push(ult_t);
        is_high = true;
        count++;
        /*
          #ifdef DEBUG
          Serial.print(ult_times.std() * 3, 4);
          Serial.print("\t<=\t");
          Serial.println(ult_times.mean() * 0.02, 4);
          Serial.print("ultimate period[us]:\t");
          Serial.println(ult_times.mean());
          #endif*/
        if (ult_times.std() * 3 <= ult_times.mean() * 0.02 && count >= 10)
        {
          is_steady_state = true;
        }
        amplitude.push(max_input - min_input);
      }
      if (min_input > input_av)
        min_input = input_av;
    }
  }
  *_t = (ult_times.mean() / 2 - *_l * 2) / log((steady_input + hys - (_perc - d_perc) * *_k) / (steady_input - hys - (_perc - d_perc) * *_k)) / 1000000.0;
  *_l = *_l / 1000000.0;
  return true;
}
