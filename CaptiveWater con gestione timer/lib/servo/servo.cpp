#include "servo.h"

Servo servo;

void openServo()
{
  servo.attach(SERVO_PIN);
  for (int posDegrees = 0; posDegrees <= OPENED_SERVO; posDegrees++)
  {
    servo.write(posDegrees);
    Serial.println(servo.read());
    delay(5);
  }
  // servo.detach();
}

void closeServo()
{
  servo.attach(SERVO_PIN);
  Serial.print("Il servo è a: ");
  Serial.println(servo.read());
  if(!(servo.read() <= CLOSED_SERVO+10 && servo.read() >= CLOSED_SERVO-10) && servo.read() != 9086){
    //se il servo non è chiuso
    for (int posDegrees = OPENED_SERVO; posDegrees >= CLOSED_SERVO; posDegrees--)
    {
      servo.write(posDegrees);
      Serial.println(servo.read());
      delay(5);
    }
  
  }
  servo.detach();
}