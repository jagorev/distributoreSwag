#include "AP.h"

DNSServer dnsServer;
AsyncWebServer server(80);

char *ssid = "distributoreSwag";
char *password = NULL;

// main web site page
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

// landing page after the choice has been made
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

// variables for managing the choice of the water size
bool isChoiceMade = false;
AcquaSize chosenSize;

// function to disconnect the clients connected to the AP and swaffle the AP channel
void refreshAP()
{
    WiFi.softAPdisconnect();
    WiFi.softAP(ssid, password, random(1, 12), 0, 1);
}

void setupServer()
{
    // if the client connects to the root of the server, send the index_html
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              {
      request->send_P(200, "text/html", index_html); 
      isChoiceMade=false;
      Serial.println("Client Connected"); });

    // in case the client connects to /get through a form submission, send the landing_html and save the choice
    server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request)
              {
      String inputMessage;
      String inputParam;

      // note that the parameters are the ones that have been named in the HTML part, so we have to refer to their names to extract them
      if (request->hasParam("bottle")) {
        inputMessage = request->getParam("bottle")->value();
        inputParam = "bottle";
        chosenSize = (AcquaSize)inputMessage.toInt();
        Serial.println(inputMessage);
        isChoiceMade = true;
      }   
      request->send(200, "text/html", landing_html); });
}
