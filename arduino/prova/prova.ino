
#include "System.h"


#define PIN 13
float arr[]={2.2 , -5};
System s(2,2);
Queue p(3),c(3,5.1);
void setup() {
    Serial.begin(115200);
    while(!Serial);
    
    pinMode(PIN, OUTPUT);
    //Serial.println(p[0]);
    p.push(2.2);
    //p.print_();
    
    p.push(2.5);
    
    Queue d(p.get_size(),p.get_elements());
    d.print_();
    
    /*p.print_();
    Serial.println(p[2]);
    Serial.println(p*c);*/
    s.set_in_coeff(arr);
    s.set_out_coeff(arr);
    
    
}

void loop() {
  Serial.println(s.out(2.2));
  delay(500);
}
