#include "servo.h"

Servo servo; // create servo object to control a servo

void openServo()
{
  servo.attach(SERVO_PIN);
  for (int posDegrees = 0; posDegrees <= OPENED_SERVO; posDegrees++) // goes from 0 degrees to OPENED_SERVO degrees
  {
    servo.write(posDegrees);
    delay(5);
  }
}

void closeServo()
{
  servo.attach(SERVO_PIN);
  //if the servo is not closed
  if(!(servo.read() <= CLOSED_SERVO+10 && servo.read() >= CLOSED_SERVO-10) && servo.read() != 9086){ 
    //close it
    for (int posDegrees = OPENED_SERVO; posDegrees >= CLOSED_SERVO; posDegrees--) // goes from OPENED_SERVO degrees to 0 degrees
    {
      servo.write(posDegrees);
      delay(5);
    }  
  }
  servo.detach();
}