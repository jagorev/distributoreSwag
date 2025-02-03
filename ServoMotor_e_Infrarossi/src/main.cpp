#include <Arduino.h>
#include <ESP32Servo.h>

static const int servoPin = 13;
static const int ANALOG_PIN = 12;

Servo servo1;

int analogValue();
int accendiServo();

void setup() {

  Serial.begin(115200);
  servo1.attach(servoPin);
}

void loop() {

 int lettura = analogValue();
  if(lettura < 30){
    accendiServo();
  }
  if (lettura > 30){
    servo1.write(45);
  }
}

int accendiServo(){
  for(int posDegrees = 45; posDegrees <= 60; posDegrees++) {
      servo1.write(posDegrees);
      //Serial.println(posDegrees);
      delay(10);
    }

    for(int posDegrees = 60; posDegrees >= 45; posDegrees--) {
      servo1.write(posDegrees);
      //Serial.println(posDegrees);
      delay(20);
    }
    servo1.write(45);
}

int analogValue(){ //funzione per leggere il valore dell'ingresso analogico (sensore infrarossi)
  int value = analogRead(ANALOG_PIN);
  Serial.println(value);
  return value;
}