#ifndef CONFIG_H
#define CONFIG_H

#define DEBUG	//enable debug mode if not commented
#include "Arduino.h"
#include "EEPROM.h"

//communication standard: "?x/x/x!"
#define SEPARATOR '/'
#define STARTER "?"
#define ENDER '!'
#define OK_MSG "?OK!"
#define ERR_MSG "?ER!"
#define NOT_INI_EEP "?NE!"
#define INI_EEP "?IE!"

#define EEPROM_SIZE sizeof(float)*5+sizeof(int)*1  //24 bytes for PID

#endif
