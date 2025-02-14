#include <Arduino.h>
#include <ESP32Servo.h>

// Definizione pin
const int trigPin = 4;
const int echoPin = 16;
const int servoPin = 13;
const int redPin = 25;
const int greenPin = 26;
const int bluePin = 27;
const int buzzerPin = 14; // Buzzer
const int stopButtonPin = 33; // Pulsante di stop

const int minDistance = 5;
const int maxDistance = 15;
bool erogazioneInCorso = false;
float acquaErogata = 0.0;
unsigned long tempoInizioErogazione = 0;
const unsigned long timeoutRipresa = 5000;

float duration_us, distance_cm;
Servo servo;

//dichiarazioni timer




void setup() {
    Serial.begin(115200);
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);
    pinMode(buzzerPin, OUTPUT);
    pinMode(stopButtonPin, INPUT_PULLUP);
    servo.attach(servoPin);
    servo.write(0);
}

float getDistance() {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    float duration_us = pulseIn(echoPin, HIGH);
    return duration_us * 0.034 / 2;
}

void setLEDColor(int r, int g, int b) {
    analogWrite(redPin, r);
    analogWrite(greenPin, g);
    analogWrite(bluePin, b);
}

void iniziaErogazione() {
    erogazioneInCorso = true;
    acquaErogata = 0.0;
    tempoInizioErogazione = millis();
    setLEDColor(0, 255, 0); // Verde
    servo.write(90);
}

void interrompiErogazione() {
    erogazioneInCorso = false;
    setLEDColor(255, 255, 0); // Giallo
    servo.write(0);
}

void fineErogazione() {
    Serial.println("FINITO");
    tone(buzzerPin, 1000, 1000); // Suona per 1 secondo
    servo.write(0);
    erogazioneInCorso = false;
}

void resetSistema() {
    Serial.println("RESET");
    setLEDColor(255, 0, 0); // Rosso
    delay(2000);
    setLEDColor(0, 0, 0);
    servo.write(0);
    erogazioneInCorso = false;
}

void loop() {
    float distance = getDistance();
    Serial.print("Distanza: ");
    Serial.println(distance);

    if (digitalRead(stopButtonPin) == LOW) {
        resetSistema();
        return;
    }

    if (!erogazioneInCorso && distance >= minDistance && distance <= maxDistance) {
        Serial.println("Borraccia rilevata! Apertura valvola...");
        iniziaErogazione();
    }

    if (erogazioneInCorso) {
        acquaErogata += 0.5;
        Serial.print("Acqua erogata: ");
        Serial.print(acquaErogata);
        Serial.println(" ml");
    }

    if (acquaErogata >= 250.0) { //controllare la quantità
        fineErogazione();
    }

    if (erogazioneInCorso && (distance > maxDistance || distance < minDistance)) {
        interrompiErogazione();
        unsigned long tempoInterruzione = millis();
        while ((millis() - tempoInterruzione) < timeoutRipresa) {
            if (getDistance() >= minDistance && getDistance() <= maxDistance) {
                iniziaErogazione();
                break;
            }
        }
    }

    delay(500);
}
