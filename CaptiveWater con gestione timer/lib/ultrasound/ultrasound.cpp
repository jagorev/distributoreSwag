#include "ultrasound.h"
#include <Arduino.h>

float getDistance()
{
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // measure duration of pulse from ECHO pin
  int duration_us = pulseIn(ECHO_PIN, HIGH);

  float distance_cm = duration_us * 0.034 / 2; // Calculate distance in cm
  Serial.print("Distance: ");
  Serial.print(distance_cm);
  Serial.println(" cm");
  return distance_cm;
}