#include "Function.h"

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
      strIdx[1] = (i == maxIdx) ? i + 1 : i;
    }
  }
  return found > index ? str.substring(strIdx[0], strIdx[1]) : "";
}


String* split(String _str, char _sep, int* _len)
{
  String msg;
  *_len = 0;
  do
  {
    msg = splitString(_str, _sep, *_len);
    *_len = *_len + 1;
  } while (msg != "");
  *_len = *_len - 1;
  String* strs = new String[*_len];
  for (int i = 0; i < *_len; i++)
  {
    strs[i] = splitString(_str, _sep, i);
  }
  return strs;
}

void clear_in_buffer()
{
  while (Serial.available() > 0) {
    char t = Serial.read();
  }
}

String* read_msg(int* _len)
{
  String* strs;
  bool is_first = true;
  String msg;
  do
  {
    if (!is_first)
    {
      Serial.print(ERR_MSG);
    }
    is_first = false;
    while (!Serial.available());
    msg = Serial.readStringUntil(ENDER);
    msg = msg.substring(0, msg.length());

#ifdef DEBUG
    Serial.println("msg received without ender");
    Serial.println(msg);
#endif

    clear_in_buffer();
    strs = split(msg, SEPARATOR, _len);
  } while (strs[0] != STARTER);
  strs = &(strs[1]);
  *_len -= 1;
  return strs;
}

void wait_serial()
{
  while (!Serial.available()) {   //wait for an incoming communication
      ;
    }
    clear_in_buffer();            //clear the serial buffer
}
