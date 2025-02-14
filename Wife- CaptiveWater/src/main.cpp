#include <DNSServer.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include "ESPAsyncWebServer.h"
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Servo.h>

DNSServer dnsServer;
AsyncWebServer server(80);

const char* ssid = "distributoreSwag";
const char* password = NULL;

enum AcquaSize {cl25, cl33, cl50, l1};

//led RGB
int rossoPin = 19;
int verdePin = 18;
int bluPin = 17;

//DISPLAY
int lcdColumns = 16;
int lcdRows = 2;
// Create an LCD object with the I2C address, columns, and rows
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

//VARIABILI UTRASUONI 
bool erogazioneInCorso = false;
float acquaErogata = 0.0; // Quantità d'acqua erogata in millilitri
unsigned long tempoInizioErogazione = 0;
const unsigned long timeoutRipresa = 5000; // 5 secondi per riposizionare la borraccia
const int trigPin = 5;     // Pin trigger sensore ultrasuoni
const int echoPin = 4;    // Pin echo sensore ultrasuoni
const int servoPin = 2;   // Pin del servomotore
const float minDistance = 0.0;  // Distanza minima in cm
const float maxDistance = 5.0; // Distanza massima in cm

Servo servo;
const int servoChiuso=0;
const int servoAperto=90;


bool scelta_effettuata = false;   
AcquaSize acqua_scelta;


//dichiarazione timer
int timerInizio = 0;
int tempoPassato = 0;
const int tempo_25cl=1000;
const int tempo_33cl=1500;
const int tempo_50cl=2000;
const int tempo_1l=4000;


const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>My ESP32 Website</title>
    <link rel="stylesheet" href="styles.css">
    <style>
        body {
            display: flex;
            flex-direction: column;
            align-items: center;
            justify-content: center;
            min-height: 100vh;
            margin: 0;
            font-family: Arial, sans-serif;
        }
        main {
            width: 90%;
            max-width: 600px;
        }
        .selectable {
            height: 80px;
            width: 80px;
            margin: 10px;
            font-size: 1em;
            background-color: white;
            border: 1px solid black;
        }
        #confirmButton {
            height: 50px;
            width: 100px;
            display: block;
            margin: 20px auto;
        }
        .selected {
            background-color: lightblue;
        }
        @media (min-width: 768px) {
            .selectable {
                height: 100px;
                width: 100px;
                font-size: 1.2em;
            }
        }
    </style>
</head>
<body>
<form action="/get">
    <header>
        <h1 style="text-align: center; color: blue;">DISTRIBUTORE DI ACQUA SWAG</h1>
    </header>
    <main>
        <div style="text-align: center;">
            <button type="submit" name="bottle" value="0">25CL</button>
            <button type="submit" name="bottle" value="1">33CL</button>
            <button type="submit" name="bottle" value="2">50CL</button>
            <button type="submit" name="bottle" value="3">1L</button>
        </div>
        <div>
        </div>
        <div id="bottle-container" style="text-align: center; margin-top: 20px;"></div>
    </main>
    <footer>
    </footer>
  </form>
</body>
</html>
)rawliteral";

void rgb(int rosso, int verde, int blu){
  analogWrite(rossoPin, rosso);
  analogWrite(verdePin, verde);
  analogWrite(bluPin, blu);
}

//funzione display
void scriviDisplay(String linea1, String linea2){

  lcd.setCursor(0, 0); // Set the cursor to the first column, first row
  lcd.print(linea1); // Print message on the first row
  lcd.setCursor(0, 1); // Set the cursor to the first column, second row
  lcd.print(linea2); // Print message on the second row 
}

void apriServo(){
  servo.attach(servoPin);
  for(int posDegrees = 0; posDegrees <= servoAperto; posDegrees++) {
    servo.write(posDegrees);
    Serial.println(servo.read());
    delay(5);
  }
  //servo.detach();
}

void chiudiServo(){
  servo.attach(servoPin);
  for(int posDegrees = servoAperto; posDegrees >= servoChiuso; posDegrees--) {
    servo.write(posDegrees);
    Serial.println(servo.read());
    delay(5);
  }
  servo.detach();
}

void erogazioneFinita(){
  scriviDisplay("Finita                ", "l'erogazione               ");
  erogazioneInCorso = false;
  rgb(255, 255, 255);
  delay(3000);
  chiudiServo();
  scelta_effettuata = false;
  //FAR SUONARE BUZZER PER AVVISARE
}

void erogaErogazione() {
  Serial.print("Acqua scelta:  ");
  Serial.println(acqua_scelta);

  tempoPassato = millis() - timerInizio;

  //caso in cui parto dall'inizio
  if(tempoPassato = 0){
    timerInizio = millis();
  }
  else{
    switch (acqua_scelta)
    {
    case cl25:
      if(tempoPassato >= tempo_25cl){
        erogazioneFinita();
      }
      break;
    case cl33:
      if(tempoPassato >= tempo_33cl){
        erogazioneFinita();
      }
      break;
    case cl50:
      if(tempoPassato >= tempo_50cl){
        erogazioneFinita();
      }
      break;
    case l1:
      if(tempoPassato >= tempo_1l){
        erogazioneFinita();
      }
      break;
    default:
      break;
    }
  }


  Serial.print("Erogazione in corso");
  Serial.println(servo.read());
  erogazioneInCorso = true;
  //tempoInizioErogazione = millis();
  rgb(0, 255, 0); // LED Verde
  if(servo.read() < servoAperto-10){
    apriServo(); // Posizione per apertura valvola
    delay(50);
  }
}
void interrompiErogazione() {
  Serial.print("Erogazione non in corso");
  Serial.println(servo.read());
  erogazioneInCorso = false;
  rgb(255, 255, 0); // LED Giallo
  if(servo.read() > servoChiuso+10){
    chiudiServo(); // Chiude l'erogatore
    delay(50);
  }
}



// void riprendiErogazione(){
//   erogazioneInCorso = true;
//   rgb(0, 255, 0); // LED Verde
//   if(servo.read() != servoAperto){
//     apriServo(); // Posizione per apertura valvola
//   }
// }

class CaptiveRequestHandler : public AsyncWebHandler {
public:
  CaptiveRequestHandler() {}
  virtual ~CaptiveRequestHandler() {}

  //ritorna true se la richiesta può essere gestita da questo gestore (sempre)
  bool canHandle(AsyncWebServerRequest *request){
    //request->addInterestingHeader("ANY");
    return true;
  }

  //è quella parte di programma che effettivamente manda ciò che verrà aperto dal captive portal (ossia index_html)
  void handleRequest(AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html); 
  }
};

void setupServer(){
  //in caso di richiesta di apertura della root page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send_P(200, "text/html", index_html); 
      scelta_effettuata=false;
      Serial.println("Client Connected");
  });
    
  //in caso di form submission
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
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
      request->send(200, "text/html", "Inserisci la bottiglia <br><a href=\"/\">Return to Home Page</a>");
  });
}




float getDistance() {
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


// void interrompiErogazione() {
//   erogazioneInCorso = false;
//   setLEDColor(255, 255, 0); // LED Giallo
//   servo.write(0); // Chiude l'erogatore
// }


void setup(){
  //your other setup stuff...
  Serial.begin(115200);
  Serial.println();

  // // Initialize the output variables as outputs
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.println("Setting up AP Mode");
  WiFi.mode(WIFI_AP); 
  WiFi.softAP(ssid, password, 1, 0, 1);
  Serial.print("AP IP address: ");Serial.println(WiFi.softAPIP());

  Serial.println("Setting up Async WebServer");
  setupServer();

  Serial.println("Starting DNS Server");
  dnsServer.start(53, "*", WiFi.softAPIP());
  server.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER);//only when requested from AP
  
  //more handlers...
  server.begin();
  Serial.println("All Done!");


  //SET UP DISPLAY
  lcd.init(); // Initialize the LCD
  lcd.backlight(); // Turn on the LCD backlight

  //SET UP SERVO
  //servo.attach(servoPin);
  //Serial.print("Servo read:");
  /*servo.write(servoChiuso);
  if(servo.read() != servoChiuso){
    chiudiServo(); 
  }*/
  chiudiServo();
  
}



void loop(){

  // if(scelta_effettuata){
  //   if(distance > maxDistance || distance < minDistance){  //se la distanza è compresa tra questo range e la scelta è stata effettuata allora si può erogare 
  //     switch (acqua_scelta)
  //     {
  //     case cl25:
    
  //       break;
  //       case cl33:

  //       break;
  //       case cl50:

  //       break;
        
  //       case l1:

  //       break;
  //     default:
  //       break;
  //     }
  //   }
  // }


  int clientCount = WiFi.softAPgetStationNum();
  //Serial.println(clientCount);
  if(clientCount == 0){
    rgb(0, 0, 255);
    scriviDisplay("Pronto                ", "collegarsi                ");
    delay(10);
    scelta_effettuata = false;
  }
  
  else if(!scelta_effettuata){
    rgb(255, 0, 255);
    scriviDisplay("Connesso:                ", "scegli taglia                ");
    delay(100);
  }

  dnsServer.processNextRequest();

  if(scelta_effettuata){
    float distance = getDistance();
    if(distance >= minDistance && distance <= maxDistance){     
      scriviDisplay("Borraccia                 ", "rilevata                ");
      erogaErogazione();
    }
    else if(distance <= minDistance || distance >= maxDistance){ 
      scriviDisplay("Metti la                ", "borraccia                ");
      interrompiErogazione();
    }




    // if(!erogazioneInCorso){
    //   rgb(255, 255, 0);
    //   scriviDisplay("Metti la                ", "borraccia                ");
    // }
    // Serial.println("Nel loop scelta_effettuata");
    

    // if(!erogazioneInCorso && distance >= minDistance && distance <= maxDistance) {
    //     Serial.println("Borraccia rilevata! Apertura valvola...");
    //     scriviDisplay("Borraccia                 ", "rilevata                ");
    //     iniziaErogazione(); // Valvola chiusa inizialmente
    //     //rgb(0,255,0); //led verde
    // }
    // if(erogazioneInCorso && distance <= minDistance && distance >= maxDistance){
    //   scriviDisplay("Metti la                ", "borraccia                ");
    //   interrompiErogazione();
    // }
    
    //   delay(100);
    //   Serial.print("Size: ");Serial.println(acqua_scelta);
      //Serial.println("We'll wait for the next client now");
  }

 
  
}