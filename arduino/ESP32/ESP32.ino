
#include "System.h"

#define EEPROM_SIZE sizeof(float)*20+sizeof(int)	//84 bytes

System sys();
void setup() {
	Serial.begin(115200);
	EEPROM.begin(EEPROM_SIZE);
	while(!Serial);
	
}

void loop() {

}

