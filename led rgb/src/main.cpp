/**
* Source code:
* https://www.italiantechproject.it/tutorial-arduino/led-rgb
*/

#include <Arduino.h>

int rossoPin = 40;
int verdePin = 41;
int bluPin = 42;

void rgb(int rosso, int verde, int blu);
 
void setup(){
  pinMode(rossoPin, OUTPUT);
  pinMode(verdePin, OUTPUT);
  pinMode(bluPin, OUTPUT);
}
 
void loop(){
  rgb(255, 0, 0);
  delay(1000);
  rgb(255, 255, 0);
  delay(1000);
  rgb(0, 255, 0);
  delay(1000);
  rgb(0, 255, 255);
  delay(1000);
  rgb(0, 0, 255);
  delay(1000);
  rgb(255, 0, 255);
  delay(1000);
}
 
void rgb(int rosso, int verde, int blu){
  analogWrite(rossoPin, rosso);
  analogWrite(verdePin, verde);
  analogWrite(bluPin, blu);
}
