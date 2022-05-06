
#include "Queue.h"

float* input();
void output(float* _output);

#define PIN 13

Queue<float> bello;
void setup() {
    Serial.begin(115200);
    pinMode(PIN, OUTPUT);
    bello.set(10);
    bello.push(12);
    Serial.println(bello[0]);
    
    
}

void loop() {

}

float* input()
{
    return new float {(float)analogRead(A0)};
}

void output(float* _output)
{
    analogWrite(PIN, _output[0]);
}
