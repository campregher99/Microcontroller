#include "EEPROM.h"
#define EEPROM_SIZE sizeof(int)
void setup() {
    Serial.begin(115200);
    EEPROM.begin(EEPROM_SIZE);
    EEPROM.put(0,int(-1));
    EEPROM.commit();
    Serial.println("erased!!");
    
    
    
}

void loop() {
    delay(1000);
}
