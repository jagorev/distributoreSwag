#include <Arduino.h>
#include <ESP32Servo.h>

static const int servoPin = 13;

Servo servo1;

void setup() {

  Serial.begin(115200);
  servo1.attach(servoPin);
}

void loop() {
  for(int posDegrees = 60; posDegrees <= 120; posDegrees++) {
    servo1.write(posDegrees);
    Serial.println(posDegrees);
    delay(10);
  }

  for(int posDegrees = 120; posDegrees >= 60; posDegrees--) {
    servo1.write(posDegrees);
    Serial.println(posDegrees);
    delay(20);
  }
}
