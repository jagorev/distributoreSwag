#include <DNSServer.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include "ESPAsyncWebServer.h"
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Servo.h>
// #include <avr/wdt.h>
#include <esp_system.h>
#include "pitches.h"

DNSServer dnsServer;
AsyncWebServer server(80);

const char *ssid = "distributoreSwag";
const char *password = NULL;

enum AcquaSize
{
  cl25,
  cl33,
  cl50,
  l1
};

//BUTTON
const int button = 25;
unsigned long button_time = 0;  
unsigned long last_button_time = 0; 
bool emergency_triggered = false;

// led RGB
const int rossoPin = 19;
const int verdePin = 18;
const int bluPin = 17;

// DISPLAY
const int lcdColumns = 16;
const int lcdRows = 2;
// Create an LCD object with the I2C address, columns, and rows
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

// VARIABILI UTRASUONI
bool erogazioneInCorso = false;
const int trigPin = 5;         // Pin trigger sensore ultrasuoni
const int echoPin = 4;         // Pin echo sensore ultrasuoni
const int servoPin = 2;        // Pin del servomotore
const float minDistance = 0.0; // Distanza minima in cm
const float maxDistance = 5.0; // Distanza massima in cm

// SERVO
Servo servo;
const int servoChiuso = 0;
const int servoAperto = 90;

// VARIABILI PER GESTIONE SCELTA ACQUA
bool scelta_effettuata = false;
AcquaSize acqua_scelta;

// TEMP0
const long int tempo_25cl = 10000;
const long int tempo_33cl = 15000;
const long int tempo_50cl = 20000;
const long int tempo_1l = 40000;
unsigned long int startTime;
unsigned long int elapsedTime = 0;
bool timerRunning = false;

//BUZZER
const int buzzerPin = 16;
const int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};
const int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};


//SITO
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>My ESP32 Website</title>
    <style>
        body {
            font-family: 'Poppins', sans-serif;
            text-align: center;
            background: linear-gradient(to right, #00c6ff, #0072ff);
            margin: 0;
            padding: 0;
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
        }

        .container {
            background: rgba(255, 255, 255, 0.9);
            padding: 40px;
            border-radius: 12px;
            box-shadow: 0 10px 20px rgba(0, 0, 0, 0.2);
            max-width: 500px;
            width: 100%;
        }

        h1 {
            color: #0056b3;
            font-size: 24px;
            margin-bottom: 20px;
        }

        button {
            padding: 15px 30px;
            margin: 10px;
            background-color: #0072ff;
            color: white;
            font-size: 1.1em;
            border: none;
            border-radius: 8px;
            cursor: pointer;
            transition: background-color 0.3s, transform 0.2s;
        }

        button:hover {
            background-color: #0056b3;
            transform: scale(1.05);
        }

        footer {
            margin-top: 40px;
            font-size: 0.9em;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>Swag Water Dispenser</h1>
        <form action="/get">
            <button type="submit" name="bottle" value="0">25 cl</button>
            <button type="submit" name="bottle" value="1">33 cl</button>
            <button type="submit" name="bottle" value="2">50 cl</button>
            <button type="submit" name="bottle" value="3">100 cl</button>
        </form>
        <footer>
            <p>&copy; Powered by DistributoreSwag</p>
        </footer>
    </div>
</body>
</html>
)rawliteral";

const char landing_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="it">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Inserisci la Bottiglia</title>
    <style>
        body {
            font-family: 'Poppins', sans-serif;
            text-align: center;
            background: linear-gradient(to right, #00c6ff, #0072ff);
            margin: 0;
            padding: 0;
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
        }
        .container {
            background: rgba(255, 255, 255, 0.9);
            padding: 40px;
            border-radius: 12px;
            box-shadow: 0 10px 20px rgba(0, 0, 0, 0.2);
            max-width: 400px;
            width: 100%;
        }
        h1 {
            color: #0056b3;
            font-size: 24px;
            margin-bottom: 10px;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>Insert the bottle</h1>
    </div>
</body>
</html>

)rawliteral";


unsigned long int getElapsedTime();

void refreshAP(){
  WiFi.softAPdisconnect();
  WiFi.softAP(ssid, password, random(1,12), 0, 1);
}

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

void rgb(int rosso, int verde, int blu)
{
  analogWrite(rossoPin, rosso);
  analogWrite(verdePin, verde);
  analogWrite(bluPin, blu);
}

// funzione display
void scriviDisplay(String linea1, String linea2)
{

  lcd.setCursor(0, 0); // Set the cursor to the first column, first row
  lcd.print(linea1);   // Print message on the first row
  lcd.setCursor(0, 1); // Set the cursor to the first column, second row
  lcd.print(linea2);   // Print message on the second row
}

void apriServo()
{
  servo.attach(servoPin);
  for (int posDegrees = 0; posDegrees <= servoAperto; posDegrees++)
  {
    servo.write(posDegrees);
    Serial.println(servo.read());
    delay(5);
  }
  // servo.detach();
}

void chiudiServo()
{
  servo.attach(servoPin);
  Serial.print("Il servo è a: ");
  Serial.println(servo.read());
  if(!(servo.read() <= servoChiuso+10 && servo.read() >= servoChiuso-10) && servo.read() != 9086){
    //se il servo non è chiuso
    for (int posDegrees = servoAperto; posDegrees >= servoChiuso; posDegrees--)
    {
      servo.write(posDegrees);
      Serial.println(servo.read());
      delay(5);
    }
  
  }
  servo.detach();
}

void erogazioneFinita()
{
  resetTimer();
  scriviDisplay("Finita                ", "l'erogazione               ");
  erogazioneInCorso = false;
  rgb(255, 255, 255);
  // delay(3000);
  chiudiServo();
  scelta_effettuata = false;

  //stacco i client connessi
  refreshAP();

  // FAR SUONARE BUZZER PER AVVISARE
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
    if (getElapsedTime() >= tempo_25cl - elapsedTime)
    {
      erogazioneFinita();
    }
    break;
  case cl33:
    if (getElapsedTime() >= tempo_33cl - elapsedTime)
    {
      erogazioneFinita();
    }
    break;
  case cl50:
    if (getElapsedTime() >= tempo_50cl - elapsedTime)
    {
      erogazioneFinita();
    }
    break;
  case l1:
    if (getElapsedTime() >= tempo_1l - elapsedTime)
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
  erogazioneInCorso = true;
  // tempoInizioErogazione = millis();
  rgb(0, 255, 0); // LED Verde

  if (servo.read() < servoAperto - 10)
  {
    apriServo(); // Posizione per apertura valvola
    delay(50);
  }
}

void interrompiErogazione()
{
  Serial.print("Erogazione non in corso  ");
  Serial.print(elapsedTime);
  // Serial.println(servo.read());
  erogazioneInCorso = false;
  rgb(255, 255, 0); // LED Giallo
  if (timerRunning)
  {
    stopTimer();
  }

  if (servo.read() > servoChiuso + 10)
  {
    chiudiServo(); // Chiude l'erogatore

    delay(50);
  }
}

void IRAM_ATTR isr() {
  button_time = millis();
  if (button_time - last_button_time > 250)
  {
    emergency_triggered = true;
    last_button_time = button_time;
  }
}

void suonaBuzzerFelice(){
  for (int thisNote = 0; thisNote < 8; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(buzzerPin, melody[thisNote], noteDuration);

    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(buzzerPin);
  }

}
void suonaBuzzerTriste() {
  tone(buzzerPin, 880, 500);  // La5
  delay(600);
  tone(buzzerPin, 698, 500);  // Fa5
  delay(600);
  tone(buzzerPin, 523, 500);  // Do5
  delay(600);
  noTone(buzzerPin);          // Spegne il buzzer
}

class CaptiveRequestHandler : public AsyncWebHandler
{
public:
  CaptiveRequestHandler() {}
  virtual ~CaptiveRequestHandler() {}

  // ritorna true se la richiesta può essere gestita da questo gestore (sempre)
  bool canHandle(AsyncWebServerRequest *request)
  {
    // request->addInterestingHeader("ANY");
    return true;
  }

  // è quella parte di programma che effettivamente manda ciò che verrà aperto dal captive portal (ossia index_html)
  void handleRequest(AsyncWebServerRequest *request)
  {
    request->send_P(200, "text/html", index_html);
  }
};

void setupServer()
{
  // in caso di richiesta di apertura della root page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            {
      request->send_P(200, "text/html", index_html); 
      scelta_effettuata=false;
      Serial.println("Client Connected"); });

  // in caso di form submission
  server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request)
            {
      String inputMessage;
      String inputParam;
      //nota che i parametri sono quelli che sono stati nominati nella parte HTML
      //quindi, devo fare riferimento ai loro nomi per estrarli
      if (request->hasParam("bottle")) {
        inputMessage = request->getParam("bottle")->value();
        inputParam = "bottle";
        acqua_scelta = (AcquaSize)inputMessage.toInt();
        Serial.println(inputMessage);
        scelta_effettuata = true;
      }   
      request->send(200, "text/html", landing_html); });
}

float getDistance()
{
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // measure duration of pulse from ECHO pin
  int duration_us = pulseIn(echoPin, HIGH);
  float distance_cm = duration_us * 0.034 / 2; // Conversione in cm
  Serial.print("Distanza rilevata: ");
  Serial.print(distance_cm);
  Serial.println(" cm");
  return distance_cm;
}



void setup()
{
  // your other setup stuff...
  Serial.begin(115200);
  pinMode(button, INPUT_PULLUP);
	attachInterrupt(button, isr, FALLING);

  // // Initialize the output variables as outputs
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);

  Serial.println("Setting up AP Mode");
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password, random(1,12), 0, 1);
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

  //chiudiServo();
}

void loop()
{
  if (emergency_triggered) {
    emergency_triggered = false; // Reset flag
    scriviDisplay("Emergenza!                ", "Riavvio in corso...                ");
    rgb(255, 0, 0);
    chiudiServo();

    //stacco i client connessi
    refreshAP();
    // FAR SUONARE BUZZER PER AVVISARE
    delay(5000);

    // Riavvio ESP
    esp_restart();
    return;
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
    if (distance >= minDistance && distance <= maxDistance)
    {
      scriviDisplay("Borraccia                 ", "rilevata                ");
      erogaErogazione();
    }
    else if (distance <= minDistance || distance >= maxDistance)
    {
      scriviDisplay("Metti la                ", "borraccia                ");
      interrompiErogazione();
    }
  }
}