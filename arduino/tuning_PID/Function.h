
#ifndef FUNCTION_H
#define FUNCTION_H
#include "Arduino.h"
#include "Config.h"
String splitString(String str, char sep, int index);
String* split(String _str, char _sep, int* _len);
void clear_in_buffer();
String* read_msg(int* _len);
void wait_serial();	//wait a serial communication
#endif
