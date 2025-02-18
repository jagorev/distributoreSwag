#include "logic.h"

bool isPouring = false;

void terminateWaterRelease()
{
    resetTimer(); // so the system can start counting again
    writeToDisplay("Water dispensing                ", "finished               ");
    isPouring = false;
    rgb(255, 255, 255);
    closeServo();
    isChoiceMade = false;

    // disconnect connected clients
    refreshAP();

    // play a happy sound to notify the end of the dispensing
    playHappyBuzzer();
    delay(5000);

    // restart the ESP so that we can go back to step 1
    esp_restart();
}

void activateWaterRelease()
{
    if (getElapsedTime() == 0)
    { // If the timer is not running, start it
        startTimer();
    }
    else if (!timerRunning)
    { // if the timer is not running, restart it
        restartTimer();
    }

    switch (chosenSize) // Check the chosen size: different times for different sizes
    {
    case cl25:
        if (getElapsedTime() >= TEMPO_25CL - elapsedTime) // If the time has passed, stop the dispensing
        {
            terminateWaterRelease();
        }
        break;
    case cl33:
        if (getElapsedTime() >= TEMPO_33CL - elapsedTime)
        {
            terminateWaterRelease();
        }
        break;
    case cl50:
        if (getElapsedTime() >= TEMPO_50CL - elapsedTime)
        {
            terminateWaterRelease();
        }
        break;
    case l1:
        if (getElapsedTime() >= TEMPO_100CL - elapsedTime)
        {
            terminateWaterRelease();
        }
        break;
    default:
        break;
    }

    Serial.print("Pouring in progress, time elapsed (from last timer stop): ");
    Serial.println(getElapsedTime());
    isPouring = true;
    rgb(0, 255, 0); // Green LED

    // If the servo is not in the right position (plus or minus an offset), move it
    if (servo.read() < OPENED_SERVO - 10)
    {
        openServo(); // Opens the dispenser
        delay(50);
    }
}

void stopWaterRelease()
{
    Serial.print("Stopped pouring, time elapsed: ");
    Serial.print(elapsedTime);
    isPouring = false;
    rgb(255, 255, 0); // Yellow led

    if (timerRunning)
    {
        stopTimer();
    }

    // If the servo is not in the right position (plus or minus an offset), move it
    if (servo.read() > CLOSED_SERVO + 10)
    {
        closeServo(); // close servo

        delay(50);
    }
}
