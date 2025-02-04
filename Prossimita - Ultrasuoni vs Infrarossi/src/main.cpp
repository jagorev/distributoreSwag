/*
 * This ESP32 code is created by esp32io.com
 *
 * This ESP32 code is released in the public domain
 *
 * For more detail (instruction and wiring diagram), visit https://esp32io.com/tutorials/esp32-ultrasonic-sensor
 */

#include <Arduino.h>
#define TRIG_PIN 4 // ESP32 pin GPIO23 connected to Ultrasonic Sensor's TRIG pin
#define ECHO_PIN 16 // ESP32 pin GPIO22 connected to Ultrasonic Sensor's ECHO pin
#define ANALOG_PIN 36 // ESP32 pin GPIO34 connected to Infrared Sensor's analog output pin

int analogValue();

float duration_us, distance_cm;

void setup() {
  // begin serial port
  Serial.begin (9600);

  //ULTRASUONI -----------------

  // configure the trigger pin to output mode
  pinMode(TRIG_PIN, OUTPUT);
  // configure the echo pin to input mode
  pinMode(ECHO_PIN, INPUT);
}

void loop() {

  //ULTRASUONI -----------------

  // generate 10-microsecond pulse to TRIG pin
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // measure duration of pulse from ECHO pin
  duration_us = pulseIn(ECHO_PIN, HIGH);

  // calculate the distance
  distance_cm = 0.017 * duration_us;

  // print the value to Serial Monitor
  Serial.print("Ultrasuoni - distance: ");
  Serial.print(distance_cm);
  Serial.println(" cm");

  delay(500);

  //INFRAROSSI -----------------
  analogValue();
}


int analogValue(){ //funzione per leggere il valore dell'ingresso analogico (sensore infrarossi)
  int value = analogRead(ANALOG_PIN);
  Serial.print("Infrarossi - value: ");
  Serial.println(value);
  return value;
}