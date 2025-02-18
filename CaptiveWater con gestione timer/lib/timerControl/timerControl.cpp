#include "timerControl.h"

//variables to manage the timer
int startTime;
int elapsedTime = 0;
bool timerRunning = false;

void startTimer()
{
    startTime = millis(); // Save the current time
    timerRunning = true;
    Serial.println("Timer is running!");
    Serial.println(getElapsedTime());
}
void stopTimer()
{
    // If the timer is running, stop it
    if (timerRunning)
    {
        elapsedTime += millis() - startTime; // Save the time elapsed since the last start
        timerRunning = false;
        Serial.println("Timer stopped!");
    }
}
void restartTimer()
{
    if (!timerRunning)
    {
        startTime = millis() - elapsedTime; // Restart the timer from the last saved time
        timerRunning = true;                // Set the timer as running
        Serial.println("Timer restarted!");
    }
}

void resetTimer()
{
    stopTimer();
    startTime = 0;   // Reset of the start time
    elapsedTime = 0; // Reset of the elapsed time
    Serial.println("Timer reset!");
}

unsigned long int getElapsedTime()
{
    if (timerRunning)
    {
        return millis() - startTime; // Calcola il tempo trascorso
    }
    else
    {
        return 0; // Se il timer non è attivo, restituisce 0
    }
}