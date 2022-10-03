#ifndef TUNING_H
#define TUNING_H
#include "Config.h"
#include "Function.h"
#include "Queue.h"

#define SIZE_I 100    //for slow process it should be increased

class Tuning
{
    float (*input)();
    void (*output)(float);
  public:
    Tuning() {};
    void begin_(float (*_input)(), void (*_output)(float));

  private:
    unsigned long long int search_delay(float _perc);
    unsigned long long int search_settling(float _perc, unsigned long long int _delay, float* _std, float* _max_out);
    bool areas_mtd(float _perc, unsigned long long int _sampling_time, float _max_input, float* _l, float* _t);
    float noise(float _perc);   //calculate input signal noise
    bool relay_mtd(float _perc, float _max_deriv, float* _k, float* _t, float* _l);

};

#endif
