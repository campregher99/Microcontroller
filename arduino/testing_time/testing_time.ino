unsigned long long int t;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  float input, ref;
  t=micros();
  //insert here the code to be tested
  Serial.println(ref);
  
  t=micros()-t;
  Serial.println(t);
}

void clear_in_buffer()
{
  while (Serial.available() > 0) {
    char t = Serial.read();
  }
}

String ref;
void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available())
  {
    t=micros();
    ref = Serial.readStringUntil('!').toFloat();
    clear_in_buffer();
    t=micros()-t;
    Serial.println(t);
  }
}
