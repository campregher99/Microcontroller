#ifndef CONFIG_H
#define CONFIG_H 

//#define DEBUG	//enable debug mode if not commented
#include "Arduino.h"
#include "Function.h"
#include "EEPROM.h"

//communication standard: "?x/x/x!"
#define SEPARATOR '/'
#define STARTER "?"
#define ENDER '!'
#define OK_MSG "?OK!"
#define ERR_MSG "?ER!"

#define EEPROM_SIZE sizeof(float)*5+sizeof(int)*3  //32 bytes for PID

#endif
