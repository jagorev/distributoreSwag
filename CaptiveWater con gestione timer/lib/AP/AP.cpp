#include "AP.h"

DNSServer dnsServer;
AsyncWebServer server(80);

char *ssid = "distributoreSwag";
char *password = NULL;

char index_html[] PROGMEM = R"rawliteral(
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
    
            BUTTON {
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
    
            BUTTON:hover {
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
                <BUTTON type="submit" name="bottle" value="0">25 cl</BUTTON>
                <BUTTON type="submit" name="bottle" value="1">33 cl</BUTTON>
                <BUTTON type="submit" name="bottle" value="2">50 cl</BUTTON>
                <BUTTON type="submit" name="bottle" value="3">100 cl</BUTTON>
            </form>
            <footer>
                <p>&copy; Powered by DistributoreSwag</p>
            </footer>
        </div>
    </body>
    </html>
    )rawliteral";


char landing_html[] PROGMEM = R"rawliteral(
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

// VARIABILI PER GESTIONE SCELTA ACQUA
bool scelta_effettuata = false;
AcquaSize acqua_scelta;

void refreshAP(){
    WiFi.softAPdisconnect();
    WiFi.softAP(ssid, password, random(1,12), 0, 1);
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

