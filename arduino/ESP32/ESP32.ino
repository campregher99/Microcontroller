
#include "System.h"



System sys;
void setup() {
	Serial.begin(115200);
	EEPROM.begin(EEPROM_SIZE);
	if(EEPROM.get(0,int)<0)
	{
		while(!Serial);	
		Serial.println("the system is not setted up.");
		setting_up();

	}
	
	
}

void loop() {

}


void setting_up()
{
	String* strs;
	String msg;
	int len;
	do
	{
		do
		{
			while(!Serial.available());
			msg = Serial.readStringUntil(ENDER);
			strs=split(msg,SEPARATOR,&len);	
		}while(strs[0]!=STARTER);
		int num_in=strs[1].toInt();
		int num_out=strs[2].toInt();
	}while(len!=num_in+num_out+3);
	float* input_coeff = new float[num_in];
	float* output_coeff = new float[num_out];
	for(int i = 3; i - 3 < num_in; i++)
	{
		input_coeff[i-3]=strs[i].toFloat();
	}
	for(int i = 3 + num_in; i - 3 - num_in < num_out; i++)
	{
		input_coeff[i - 3 - num_in]=strs[i].toFloat();
	}
	EEPROM.put(0,num_in+num_out+1);
	for(int i = sizeof(int); i - sizeof(int) < sizeof(float) * num_in; i = i + sizeof(float))
	{
		EEPROM.put(i,input_coeff[(i-sizeof(int))/sizeof(float)]);
	}
	for(int i = sizeof(int) + sizeof(float) * num_in; i - sizeof(int) - sizeof(float) * num_in < sizeof(float) * num_out; i = i + sizeof(float))
	{
		EEPROM.put(i,output_coeff[(i-sizeof(int) - sizeof(float) * num_in)/sizeof(float)]);
	}
	sys.set_coeffs(input_coeff,output_coeff,num_in,num_out);
}


String splitString(String str, char sep, int index)
{
 int found = 0;
 int strIdx[] = { 0, -1 };
 int maxIdx = str.length() - 1;

 for (int i = 0; i <= maxIdx && found <= index; i++)
 {
    if (str.charAt(i) == sep || i == maxIdx)
    {
      found++;
      strIdx[0] = strIdx[1] + 1;
      strIdx[1] = (i == maxIdx) ? i+1 : i;
    }
 }
 return found > index ? str.substring(strIdx[0], strIdx[1]) : "";
}


String* split(String _str,char _sep, int* _len)
{
  String msg;
  *_len = 0;
  do
  {
    msg=splitString(_str,_sep,*_len);
    *_len=*_len+1;
  }while(msg!="");
  *_len=*_len-1;
  String* strs = new String[*_len];
  for(int i = 0; i < *_len; i++)
  {
    strs[i]=splitString(_str,_sep,i);
  }
  return strs;
}