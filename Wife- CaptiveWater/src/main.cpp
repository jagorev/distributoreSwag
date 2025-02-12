#include <DNSServer.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include "ESPAsyncWebServer.h"
#include <Arduino.h>

DNSServer dnsServer;
AsyncWebServer server(80);

const char* ssid = "distributoreSwag";
const char* password = NULL;
enum AcquaSize {cl25, cl33, cl50, l1};

//led RGB
int rossoPin = 42;
int verdePin = 41;
int bluPin = 40;

bool scelta_effettuata = false;
AcquaSize acqua_scelta;

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
            <button type="submit" name="bottle" value="0">25</button>
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


void setup(){
  //your other setup stuff...
  Serial.begin(115200);
  Serial.println();

  // // Initialize the output variables as outputs
  pinMode(rossoPin, OUTPUT);
  pinMode(verdePin, OUTPUT);
  pinMode(bluPin, OUTPUT);

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
}

void loop(){
  int clientCount = WiFi.softAPgetStationNum();
  Serial.println(clientCount);
  if(clientCount == 0){
    rgb(0, 0, 255);
    delay(10);
    scelta_effettuata = false;
  }
  else if(!scelta_effettuata){
    rgb(255, 0, 255);
    delay(10);
  }
  dnsServer.processNextRequest();
  if(scelta_effettuata){
    rgb(255, 255, 0);
    delay(10);
      Serial.print("Size: ");Serial.println(risultato);
      Serial.println("We'll wait for the next client now");
  }
  
}