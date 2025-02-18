#include "logic.h"

bool isPouring = false;

void erogazioneFinita()
{
    resetTimer();
    scriviDisplay("Finita                ", "l'erogazione               ");
    isPouring = false;
    rgb(255, 255, 255);
    // delay(3000);
    closeServo();
    scelta_effettuata = false;

    // stacco i client connessi
    refreshAP();

    playHappyBuzzer();
    delay(5000);

    esp_restart();
}

void erogaErogazione()
{
    Serial.print("Acqua scelta:  ");
    Serial.println(acqua_scelta);

    if (getElapsedTime() == 0)
    { // Se il tempo trascorso è 0, allora il timer non è mai partito
        startTimer();
    }
    else if (!timerRunning)
    { // se il timer non è attivo lo riavvia
        restartTimer();
    }
    switch (acqua_scelta)
    {
    case cl25:
        if (getElapsedTime() >= TEMPO_25CL - elapsedTime)
        {
            erogazioneFinita();
        }
        break;
    case cl33:
        if (getElapsedTime() >= TEMPO_33CL - elapsedTime)
        {
            erogazioneFinita();
        }
        break;
    case cl50:
        if (getElapsedTime() >= TEMPO_50CL - elapsedTime)
        {
            erogazioneFinita();
        }
        break;
    case l1:
        if (getElapsedTime() >= TEMPO_100CL - elapsedTime)
        {
            erogazioneFinita();
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

void interrompiErogazione()
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
