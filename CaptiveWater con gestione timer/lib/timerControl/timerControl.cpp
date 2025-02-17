#include "timerControl.h"

int startTime;
int elapsedTime = 0;
bool timerRunning = false;

void startTimer()
{
    startTime = millis();
    timerRunning = true;
    Serial.println("Timer partito!");
    Serial.println(getElapsedTime());
}
void stopTimer()
{
    if (timerRunning)
    {
        elapsedTime += millis() - startTime;
        timerRunning = false;
        Serial.println("Timer fermato!");
    }
}
void restartTimer()
{
    if (!timerRunning)
    {
        startTime = millis() - elapsedTime; // Riprendi da dove era stato interrotto
        timerRunning = true;                // Rende il timer attivo
        Serial.println("Timer riavviato!");
    }
}

void resetTimer()
{
    stopTimer();
    startTime = 0;   // Reset del tempo di inizio
    elapsedTime = 0; // Reset del tempo trascorso
    Serial.println("Timer resettato!");
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