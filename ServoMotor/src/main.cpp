#include <Arduino.h>
#include <ESP32Servo.h>

static const int servoPin = 2;

Servo servo;

void setup() {

  Serial.begin(115200);
  servo.attach(servoPin);
  for(int posDegrees = 90; posDegrees >= 10; posDegrees--) {
    servo.write(posDegrees);
    //Serial.println(posDegrees);
    Serial.println(servo.read());
    delay(5);
  }
  servo.detach();
}

void loop() {
  // for(int posDegrees = 0; posDegrees <= 90; posDegrees++) {
  //   servo.write(posDegrees);
  //   //Serial.println(posDegrees);
  //   Serial.println(servo.read());
  //   delay(5);
  // }

  
}
