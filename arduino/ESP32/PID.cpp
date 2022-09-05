#include "PID.h"

long unsigned int begin_(PID* _pid, float * (*_input)(), void (*_output)(float*))
{
  _pid->input = _input;
  _pid->output = _output;
  Serial.begin(115200);
  EEPROM.begin(EEPROM_SIZE);
  int num_in, num_out, index;
  EEPROM.get(0, index);
  Serial.println(index);
  long unsigned int Ts;
  if (index != 32)
  {
    setEEPROM();
    long long unsigned int times;
    times = micros();
    refresh();
    times = micros() - times;
    Serial.print(STARTER);
    Serial.print(times);
    Serial.println(ENDER);
    setEEPROM();
    String* strs;
    int len;
    strs = read_msg(&len);
    Ts = strs[0].toInt();
    Serial.println(OK_MSG);
    Serial.println(Ts);
  }
  else
  {
    Ts=2000000;
    EEPROM.get(sizeof(int), num_in);
    EEPROM.get(sizeof(int) * 2, num_out);
    
    float* input_coeff = new float[num_in];
    float* output_coeff = new float[num_out];
    float store;
    
    for (int i = sizeof(int) * 3; i - sizeof(int) * 3 < sizeof(float) * num_in; i = i + sizeof(float))
    {
      EEPROM.get(i, store);
      input_coeff[i]=store;
    }
    
    for (int i = sizeof(int) * 3 + sizeof(float) * num_in; i - sizeof(int) * 3 - sizeof(float) * num_in < sizeof(float) * num_out; i = i + sizeof(float))
    {
      EEPROM.get(i, output_coeff[i]);
    }
    Serial.println("Entro in preinitialized way");
    pid.set_coeffs(input_coeff, output_coeff, num_in, num_out);
  }
  return Ts;
}


void refresh(PID* _pid)
{
  float *in, out;
  in = _pid->input();
  out = _pid->pid.out(in[0]);
  _pid->output(&out);
}

void setEEPROM()
{
  int num_in, num_out;
  String* strs;
  int len;
  do
  {
    strs = read_msg(&len);
    Serial.println(strs[0]);
    num_in = strs[1].toInt();
    num_out = strs[2].toInt();
  } while (len != num_in + num_out + 3 + 1);
  float* input_coeff = new float[num_in];
  float* output_coeff = new float[num_out];
  for (int i = 3; i - 3 < num_in; i++)
  {
    input_coeff[i - 3] = strs[i].toFloat();
  }
  for (int i = 3 + num_in; i - 3 - num_in < num_out; i++)
  {
    input_coeff[i - 3 - num_in] = strs[i].toFloat();
  }
  Serial.println((int)(num_in*sizeof(float) + num_out*sizeof(float) + 3*sizeof(int)));
  EEPROM.put(0, (int)(num_in*sizeof(float) + num_out*sizeof(float) + 3*sizeof(int)));
  EEPROM.put(sizeof(int), num_in);
  EEPROM.put(sizeof(int) * 2, num_out);
  for (int i = sizeof(int) * 3; i - sizeof(int) * 3 < sizeof(float) * num_in; i = i + sizeof(float))
  {
    EEPROM.put(i, input_coeff[(i - sizeof(int)) / sizeof(float)]);
  }
  for (int i = sizeof(int) * 3 + sizeof(float) * num_in; i - sizeof(int) * 3 - sizeof(float) * num_in < sizeof(float) * num_out; i = i + sizeof(float))
  {
    EEPROM.put(i, output_coeff[(i - sizeof(int) - sizeof(float) * num_in) / sizeof(float)]);
  }
  pid.set_coeffs(input_coeff, output_coeff, num_in, num_out);
  EEPROM.commit();
}

String* read_msg(int* _len)
{
  String* strs;
  bool is_first = true;
  String msg;
  do
  {
    if (!is_first)
    {
      Serial.print(ERR_MSG);
    }
    is_first = false;
    while (!Serial.available());
    msg = Serial.readStringUntil(ENDER);
    clear_in_buffer();
    strs = split(msg, SEPARATOR, _len);
  } while (strs[0] != STARTER);
  strs = &(strs[1]);
  return strs;
}
