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
#include "logic.h"

//interrupt service routine function
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

void setup()
{
  //Initialize Serial port
  Serial.begin(115200);

  //Specify the pin modes
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  //Attach the interrupt for the button pressing event
  attachInterrupt(BUTTON, isr, FALLING);

  //Set up the ESP as an Access Point
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password, random(1, 12), 0, 1); // Create an open network with a random channel and max clients = 1
  Serial.print("AP IP address: "); // Print the IP address of the AP
  Serial.println(WiFi.softAPIP());

  //set up the Async Web Server, this way we can handle the requests from the captive portal
  setupServer();

  //set up the DNS server, this way we can redirect all the requests to the captive portal
  dnsServer.start(53, "*", WiFi.softAPIP()); // redirect all requests to the captive portal (AP IP)
  server.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER); // only when requested from AP
  server.begin();

  //Set up the display
  lcd.init();      // Initialize the LCD
  lcd.backlight(); // Turn on the LCD backlight
}

void loop()
{
  //Check if the emergency button has been pressed (emergencyTriggered is set as true by the ISR defined above)
  if (emergencyTriggered)
  {
    managingEmergency();
  }

  //Check if the client has connected to the AP
  int clientCount = WiFi.softAPgetStationNum();
  if (clientCount == 0)
  {
    rgb(0, 0, 255);
    writeToDisplay("Ready                ", "to connect                ");
    delay(10);
    isChoiceMade = false;
  }

  else if (!isChoiceMade)
  {
    rgb(255, 0, 255);
    writeToDisplay("Connected:                ", "choose size                ");
    delay(100);
  }

  dnsServer.processNextRequest();

  if (isChoiceMade)
  {
    float distance = getDistance();
    if (distance >= MIN_DISTANCE && distance <= MAX_DISTANCE)
    {
      writeToDisplay("Water bottle                 ", "detected                ");
      activateWaterRelease();
    }
    else if (distance <= MIN_DISTANCE || distance >= MAX_DISTANCE)
    {
      writeToDisplay("Insert the                ", "water bottle                ");
      stopWaterRelease();
    }
  }
}