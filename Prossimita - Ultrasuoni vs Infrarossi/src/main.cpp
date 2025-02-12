/*
 * This ESP32 code is created by esp32io.com
 *
 * This ESP32 code is released in the public domain
 *
 * For more detail (instruction and wiring diagram), visit https://esp32io.com/tutorials/esp32-ultrasonic-sensor
 */

#include <Arduino.h>
#include <ServoMotor.h>
#include <ESP32Servo.h>


// Definizione pin
const int trigPin = 4;     // Pin trigger sensore ultrasuoni
const int echoPin = 16;    // Pin echo sensore ultrasuoni
const int servoPin = 13;   // Pin del servomotore
const int redPin = 25;     // Pin LED RGB - Rosso
const int greenPin = 26;   // Pin LED RGB - Verde
const int bluePin = 27;    // Pin LED RGB - Blu

// Variabili per la distanza
const int minDistance = 5;  // Distanza minima in cm
const int maxDistance = 15; // Distanza massima in cm

bool erogazioneInCorso = false;
float acquaErogata = 0.0; // Quantità d'acqua erogata in millilitri
unsigned long tempoInizioErogazione = 0;
const unsigned long timeoutRipresa = 5000; // 5 secondi per riposizionare la borraccia



float duration_us, distance_cm;
Servo servo;

void setup() {
    Serial.begin(115200);
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);
    servo.attach(servoPin);
    servo.write(0); // Valvola chiusa inizialmente
}

float getDistance(float duration_us, float distance_cm) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration_us = pulseIn(echoPin, HIGH);
  distance_cm = duration_us * 0.034 / 2; // Conversione in cm
  return distance;
}


void setLEDColor(int r, int g, int b) {
  analogWrite(redPin, r);
  analogWrite(greenPin, g);
  analogWrite(bluePin, b);
}

// Funzione di erogazione acqua
void iniziaErogazione() {
  erogazioneInCorso = true;
  acquaErogata = 0.0;
  tempoInizioErogazione = millis();
  setLEDColor(0, 255, 0); // LED Verde
  servo.write(90); // Posizione per apertura valvola
}

void interrompiErogazione() {
  erogazioneInCorso = false;
  setLED(255, 255, 0); // LED Giallo
  servo.write(0); // Chiude l'erogatore
}
  

void loop() {
  float distance = getDistance();
  Serial.print("Distanza rilevata: ");
  Serial.print(distance);
  Serial.println(" cm");

 
  if (!erogazioneInCorso && distance >= minDistance && distance <= maxDistance) {
      Serial.println("Borraccia rilevata! Apertura valvola...");
      iniziaErogazione();
  }

  if (erogazioneInCorso) {
    acquaErogata += 0.5; // Supponiamo che ogni iterazione eroghi 0.5 ml
    Serial.print("Acqua erogata: ");
    Serial.print(acquaErogata);
    Serial.println(" ml");
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

  delay(500); // Piccola pausa per stabilità
}



