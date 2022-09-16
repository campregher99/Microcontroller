#ifndef CONFIG_H
#define CONFIG_H

//#define DEBUG	//enables debug mode if not commented
//#define MONITOR //enables monitor mode if not commented
#include "Arduino.h"

//communication standard: "?/x/x/x!"
#define SEPARATOR '/'
#define STARTER "?"
#define ENDER '!'
#define OK_MSG "?OK!"
#define ERR_MSG "?ER!"

//PWM parameter
#define FREQ_PWM 5000
#define CHANNEL_PWM 0
#define RESOLUTION_PWM 8
#define PIN_PWM 16

#define E 2.718281828

#endif
