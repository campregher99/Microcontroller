#include "Controller.h"

uint64_t Controller::begin_(float (*_input)(), void (*_output)(float))
{
  //initialize input/output functions
  input = _input;
  output = _output;

  //communications initialization
  Serial.begin(115200);
  EEPROM.begin(EEPROM_SIZE);

  int index;
  EEPROM.get(0, index);

#ifdef DEBUG
  Serial.println("Initial number of EEPROM:");
  Serial.println(index);
#endif

  uint64_t Ts;		//sampling time

  if (index != EEPROM_SIZE)
  { //eeprom not initialized
    wait_serial();    //waiting for Serial comunication

    Serial.println(NOT_INI_EEP);
    setEEPROM();	//initial settings

    //virifies the minimum sampling time
    long long unsigned int times;
    times = micros();
    refresh();
    times = micros() - times;

    //send back the minimum sampling time
    Serial.print(STARTER);
    Serial.print(times);
    Serial.println(ENDER);

    setEEPROM();    //setting the system definitively

    //setting the actual sample time
    String* strs;
    int len;
    strs = read_msg(&len);
    Ts = strs[0].toInt();
    EEPROM.put(EEP_TS, Ts);
    EEPROM.commit();
    Serial.println(OK_MSG);

#ifdef DEBUG
    Serial.println(Ts);
#endif
  }
  else
  { //eeprom initialized

    Serial.println(INI_EEP);

    int input_size, output_size;

    EEPROM.get(sizeof(int), input_size);
    EEPROM.get(sizeof(int) * 2, output_size);

    float input_coeff[input_size];
    float output_coeff[output_size];
    float store;

    for (int i = sizeof(int) * 3; i - sizeof(int) * 3 < sizeof(float) * input_size; i = i + sizeof(float))
    {
      EEPROM.get(i, store);
      input_coeff[(i - sizeof(int) * 3) / sizeof(float)] = store;
    }

    for (int i = sizeof(int) * 3 + sizeof(float) * 20; i - sizeof(int) * 3 - sizeof(float) * 20 < sizeof(float) * output_size; i = i + sizeof(float))
    {
      EEPROM.get(i, output_coeff[(i - sizeof(int) * 3 - sizeof(float) * 20) / sizeof(float)]);
    }

    EEPROM.get(EEP_TS, Ts);

    sys.initialize(input_size, output_size);
    sys.set_coeffs(input_coeff, output_coeff);

#ifdef DEBUG
    sys.print_();
#endif
  }
  return Ts;
}

void Controller::refresh()
{
  float in, out;
  in = input();
  out = sys.out(in);

  //anti windup
  if(out<0)
  {
    out=0;
  }else if(out>100)
  {
    out=100;
  }
  output(out);
  sys.set_output(out);
#ifdef DEBUG
  Serial.println(out);
#endif
}

void Controller::setEEPROM()
{
  static bool is_first = true;
  String* strs;
  int len, input_size, output_size;
  do
  {
    strs = read_msg(&len);	//read message from serial and parse it
#ifdef DEBUG
    Serial.println(strs[0]);
    Serial.println(len);
#endif
  } while (len < 5);	//check if the message length isn't correct otherwise moves on
  input_size = strs[1].toInt();
  output_size = strs[2].toInt();
  float input_coeff[input_size];
  float output_coeff[output_size];

#ifdef DEBUG
  Serial.println("message correct");
  Serial.println("Input coeff:");
#endif

  //reading input coefficents
  for (int i = 3; i - 3 < input_size; i++)
  {
    input_coeff[i - 3] = strs[i].toFloat();
#ifdef DEBUG
    Serial.println(strs[i].toFloat());
#endif
  }

#ifdef DEBUG
  Serial.println("Output coeff:");
#endif

  //reading output coefficents
  for (int i = 3 + input_size; i - 3 - input_size < output_size; i++)
  {
    output_coeff[i - 3 - input_size] = strs[i].toFloat();
#ifdef DEBUG
    Serial.println(strs[i].toFloat());
#endif
  }

  //saving data in the eeprom
  if (is_first)
  {
    EEPROM.put(0, EEPROM_SIZE);
    EEPROM.put(sizeof(int), input_size);
    EEPROM.put(sizeof(int) * 2, output_size);
  }
  for (int i = sizeof(int) * 3; i - sizeof(int) * 3 < sizeof(float) * input_size; i = i + sizeof(float))
  {
    EEPROM.put(i, input_coeff[(i - sizeof(int) * 3) / sizeof(float)]);
  }
  for (int i = sizeof(int) * 3 + sizeof(float) * 20; i - sizeof(int) * 3 - sizeof(float) * 20 < sizeof(float) * output_size; i = i + sizeof(float))
  {
    EEPROM.put(i, output_coeff[(i - sizeof(int) * 3 - sizeof(float) * 20) / sizeof(float)]);
  }

  //settings the system coefficents
  if (is_first)
  {
    sys.initialize(input_size, output_size);
    is_first = false;
  }
  sys.set_coeffs(input_coeff, output_coeff);

#ifdef DEBUG
  sys.print_();
#endif

  //actual eeprom writing
  EEPROM.commit();
}
