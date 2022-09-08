#include "PID.h"

PID::PID()
{
  sys.initialize(INPUT_, OUTPUT_);
}

long unsigned int PID::begin_(float (*_input)(), void (*_output)(float))
{
  //initialize input/output functions
  input = _input;
  output = _output;

  //communications initialization
  Serial.begin(115200);
  EEPROM.begin(EEPROM_SIZE);
  while (!Serial) { //waiting for Serial comunication
    ;
  }

  int index;
  EEPROM.get(0, index);

#ifdef DEBUG
  Serial.println("Initial number of EEPROM:");
  Serial.println(index);
#endif

  long unsigned int Ts;		//sampling time

  if (index != EEPROM_SIZE)
  { //eeprom not initialized
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
    Serial.println(OK_MSG);

#ifdef DEBUG
    Serial.println(Ts);
#endif
  }
  else
  { //eeprom initialized
    Ts = 2000000;

    Serial.println(INI_EEP);

    float input_coeff[INPUT_];
    float output_coeff[OUTPUT_];
    float store;

    for (int i = sizeof(int) * 1; i - sizeof(int) * 1 < sizeof(float) * INPUT_; i = i + sizeof(float))
    {
      EEPROM.get(i, store);
      input_coeff[(i - sizeof(int) * 1) / sizeof(float)] = store;
#ifdef DEBUG
      Serial.println(store);
#endif
    }

    for (int i = sizeof(int) * 1 + sizeof(float) * INPUT_; i - sizeof(int) * 1 - sizeof(float) * INPUT_ < sizeof(float) * OUTPUT_; i = i + sizeof(float))
    {
      EEPROM.get(i, output_coeff[(i - sizeof(int) * 1 - sizeof(float) * INPUT_) / sizeof(float)]);
    }

    sys.set_coeffs(input_coeff, output_coeff);

#ifdef DEBUG
    sys.print_();
#endif
  }
  return Ts;
}

void PID::refresh()
{
  float in, out;
  in = input();
  out = sys.out(in);
  output(out);
}

void PID::setEEPROM()
{
  String* strs;
  int len;
  do
  {
    strs = read_msg(&len);	//read message from serial and parse it
#ifdef DEBUG
    Serial.println(strs[0]);
    Serial.println(len);
#endif
  } while (len != INPUT_ + OUTPUT_ + 1);	//check if the message length isn't correct otherwise moves on
  float input_coeff[INPUT_];
  float output_coeff[OUTPUT_];

#ifdef DEBUG
  Serial.println("message correct");
  Serial.println("Input coeff:");
#endif

  //reading input coefficents
  for (int i = 1; i - 1 < INPUT_; i++)
  {
    input_coeff[i - 1] = strs[i].toFloat();
#ifdef DEBUG
    Serial.println(strs[i].toFloat());
#endif
  }

#ifdef DEBUG
  Serial.println("Output coeff:");
#endif

  //reading output coefficents
  for (int i = 1 + INPUT_; i - 1 - INPUT_ < OUTPUT_; i++)
  {
    output_coeff[i - 1 - INPUT_] = strs[i].toFloat();
#ifdef DEBUG
    Serial.println(strs[i].toFloat());
#endif
  }

  //saving data in the eeprom
  EEPROM.put(0, EEPROM_SIZE);
  EEPROM.put(sizeof(int), INPUT_);
  EEPROM.put(sizeof(int) * 2, OUTPUT_);
  for (int i = sizeof(int) * 1; i - sizeof(int) * 1 < sizeof(float) * INPUT_; i = i + sizeof(float))
  {
    EEPROM.put(i, input_coeff[(i - sizeof(int)) / sizeof(float)]);
  }
  for (int i = sizeof(int) * 1 + sizeof(float) * INPUT_; i - sizeof(int) * 1 - sizeof(float) * INPUT_ < sizeof(float) * OUTPUT_; i = i + sizeof(float))
  {
    EEPROM.put(i, output_coeff[(i - sizeof(int) - sizeof(float) * INPUT_) / sizeof(float)]);
  }

  //settings the system coefficents
  sys.set_coeffs(input_coeff, output_coeff);

#ifdef DEBUG
  sys.print_();
#endif

  //actual eeprom writing
  EEPROM.commit();
}
