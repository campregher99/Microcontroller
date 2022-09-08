#include "EEPROM.h"
#define EEPROM_SIZE 24
void setup() {
  Serial.begin(115200);
  float numb;
  int n;
  EEPROM.begin(EEPROM_SIZE);
  EEPROM.get(0,n);
  Serial.println(n);
  for(int i=sizeof(int);i<EEPROM_SIZE;i+=sizeof(float))
  {
    EEPROM.get(i,numb);
    Serial.println(numb);
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
