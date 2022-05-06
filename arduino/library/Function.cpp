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
      strIdx[1] = (i == maxIdx) ? i+1 : i;
    }
 }
 return found > index ? str.substring(strIdx[0], strIdx[1]) : "";
}


String* split(String _str,char _sep, int* _len)
{
  String msg;
  *_len = 0;
  do
  {
    msg=splitString(_str,_sep,*_len);
    *_len=*_len+1;
  }while(msg!="");
  *_len=*_len-1;
  String* strs = new String[*_len];
  for(int i = 0; i < *_len; i++)
  {
    strs[i]=splitString(_str,_sep,i);
  }
  return strs;
}