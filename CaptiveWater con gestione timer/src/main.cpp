#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <esp_system.h>
#include "buzzer.h"
#include "display.h"
#include "rgb.h"
#include "servo.h"
#include "ultrasound.h"
#include "button.h"
#include "AP.h"
#include "CaptiveRequestHandler.h"
#include "timerControl.h"

// BUTTON

// led RGB

// DISPLAY

// VARIABILI UTRASUONI
bool isPouring = false;

// SERVO

// TEMP0

// BUZZER

// SITO

void IRAM_ATTR isr()
{
  buttonTime = millis();
  // we're careful about the deboucing effect that can occur
  if (buttonTime - lastButtonTime > 250)
  {
    emergencyTriggered = true;
    lastButtonTime = buttonTime;
  }
}

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

void setup()
{
  // your other setup stuff...
  Serial.begin(115200);
  pinMode(BUTTON, INPUT_PULLUP);
  attachInterrupt(BUTTON, isr, FALLING);

  // // Initialize the output variables as outputs
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  Serial.println("Setting up AP Mode");
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password, random(1, 12), 0, 1);
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  Serial.println("Setting up Async WebServer");
  setupServer();

  Serial.println("Starting DNS Server");
  dnsServer.start(53, "*", WiFi.softAPIP());
  server.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER); // only when requested from AP

  // more handlers...
  server.begin();
  Serial.println("All Done!");

  // SET UP DISPLAY
  lcd.init();      // Initialize the LCD
  lcd.backlight(); // Turn on the LCD backlight

  // closeServo();
}

void loop()
{
  if (emergencyTriggered)
  {
    managingEmergency();
  }
  int clientCount = WiFi.softAPgetStationNum();
  // Serial.println(clientCount);
  if (clientCount == 0)
  {
    rgb(0, 0, 255);
    scriviDisplay("Pronto                ", "collegarsi                ");
    delay(10);
    scelta_effettuata = false;
  }

  else if (!scelta_effettuata)
  {
    rgb(255, 0, 255);
    scriviDisplay("Connesso:                ", "scegli taglia                ");
    delay(100);
  }

  dnsServer.processNextRequest();

  if (scelta_effettuata)
  {
    float distance = getDistance();
    if (distance >= MIN_DISTANCE && distance <= MAX_DISTANCE)
    {
      scriviDisplay("Borraccia                 ", "rilevata                ");
      erogaErogazione();
    }
    else if (distance <= MIN_DISTANCE || distance >= MAX_DISTANCE)
    {
      scriviDisplay("Metti la                ", "borraccia                ");
      interrompiErogazione();
    }
  }
}