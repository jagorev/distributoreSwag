#include "logic.h"

bool isPouring = false;

void terminateWaterRelease()
{
    resetTimer();
    writeToDisplay("Water dispensing                ", "finished               ");
    isPouring = false;
    rgb(255, 255, 255);
    // delay(3000);
    closeServo();
    isChoiceMade = false;

    // stacco i client connessi
    refreshAP();

    playHappyBuzzer();
    delay(5000);

    esp_restart();
}

void activateWaterRelease()
{
    Serial.print("Chosen size:  ");
    Serial.println(chosenSize);

    if (getElapsedTime() == 0)
    { // Se il tempo trascorso è 0, allora il timer non è mai partito
        startTimer();
    }
    else if (!timerRunning)
    { // se il timer non è attivo lo riavvia
        restartTimer();
    }
    switch (chosenSize)
    {
    case cl25:
        if (getElapsedTime() >= TEMPO_25CL - elapsedTime)
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

    Serial.print("Erogazione in corso");
    Serial.println(servo.read());
    Serial.println(getElapsedTime());
    isPouring = true;
    // tempoInizioErogazione = millis();
    rgb(0, 255, 0); // LED Verde

    if (servo.read() < OPENED_SERVO - 10)
    {
        openServo(); // Posizione per apertura valvola
        delay(50);
    }
}

void stopWaterRelease()
{
    Serial.print("Erogazione non in corso  ");
    Serial.print(elapsedTime);
    // Serial.println(servo.read());
    isPouring = false;
    rgb(255, 255, 0); // LED Giallo
    if (timerRunning)
    {
        stopTimer();
    }

    if (servo.read() > CLOSED_SERVO + 10)
    {
        closeServo(); // Chiude l'erogatore

        delay(50);
    }
}
