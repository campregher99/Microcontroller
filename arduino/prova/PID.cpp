#include "PID.h"

PID::PID()
{
	sys.initialize(3,2);
}

long unsigned int PID::begin_()
{
  //communications initialization
  Serial.begin(115200);
  EEPROM.begin(EEPROM_SIZE);

  int num_in, num_out, index;
  EEPROM.get(0, index);

  #ifdef DEBUG
  Serial.println("Initial number of EEPROM:")
  Serial.println(index);
  #endif

  long unsigned int Ts;		//sampling time

  if (index != EEPROM_SIZE)
  {
    setEEPROM();	//initial settings

    //virifies the minimum sampling time
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
    sys.set_coeffs(input_coeff, output_coeff, num_in, num_out);
  }
  return Ts;
}

void PID::setEEPROM()
{
  int num_in, num_out;
  String* strs;
  int len;
  do
  {
    strs = read_msg(&len);	//read message from serial and parse it
    #ifdef DEBUG
    Serial.println(strs[0]);
    #endif
    num_in = strs[1].toInt();
    num_out = strs[2].toInt();
  } while (len != 8);	//check if the message length isn't correct otherwise moves on
  float input_coeff[3];
  float output_coeff[2];

  //reading input coefficents
  for (int i = 3; i - 3 < num_in; i++)
  {
    input_coeff[i - 3] = strs[i].toFloat();
  }
  
  //reading output coefficents
  for (int i = 3 + num_in; i - 3 - num_in < num_out; i++)
  {
    input_coeff[i - 3 - num_in] = strs[i].toFloat();
  }

  #ifdef DEBUG
  Serial.println((int)(num_in*sizeof(float) + num_out*sizeof(float) + 3*sizeof(int)));
  #endif

  //saving data in the eeprom
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

  //settings the system coefficents
  sys.set_coeffs(input_coeff, output_coeff);
  
  #ifdef DEBUG
  sys.print_();
  #endif

  //actual eeprom writing
  EEPROM.commit();
}