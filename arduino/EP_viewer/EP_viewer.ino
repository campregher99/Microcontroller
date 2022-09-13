#include "EEPROM.h"
#define EEPROM_SIZE_FL 24
#define EEPROM_SIZE 32
void setup() {
  Serial.begin(115200);
  float numb;
  uint64_t Ts;
  int n;
  EEPROM.begin(EEPROM_SIZE);
  EEPROM.get(0,n);
  Serial.println(n);
  for(int i=sizeof(int);i<EEPROM_SIZE_FL;i+=sizeof(float))
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
