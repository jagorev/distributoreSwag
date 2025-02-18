#ifndef SERVO_H
#define SERVO_H

#include <ESP32Servo.h>

#define CLOSED_SERVO 0
#define OPENED_SERVO 90
#define SERVO_PIN 2       // Pin del servomotore

extern Servo servo;

void openServo();
void closeServo();


#endif
