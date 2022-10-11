#include "EEPROM.h"
#define EEPROM_SIZE_FL 180
#define EEPROM_SIZE 188
void setup() {
  Serial.begin(115200);
  float numb;
  uint64_t Ts;
  int n;
  EEPROM.begin(EEPROM_SIZE);
  EEPROM.get(0,n);
  Serial.println(n);
  EEPROM.get(sizeof(int),n);
  Serial.println(n);
  EEPROM.get(sizeof(int)*2,n);
  Serial.println(n);
  for(int i=sizeof(int)*3;i<EEPROM_SIZE_FL;i+=sizeof(float))
  {
    EEPROM.get(i,numb);
    Serial.println(numb);
  }
  EEPROM.get(EEPROM_SIZE_FL,Ts);
  Serial.println(Ts);
}

void loop() {
  // put your main code here, to run repeatedly:

}
