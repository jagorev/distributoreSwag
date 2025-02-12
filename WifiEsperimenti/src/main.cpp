#include <Arduino.h>
#include <WiFi.h>

const char *ssid = "ESP32-Access-Point";
const char *password = "123456789";

// Set web server port number to 80
WiFiServer server(80);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password, 1, 0, 4); // Start the Access Point

  Serial.print("Setting AP (Access Point)…");
  Serial.println(WiFi.softAPIP()); // Print the IP address

  server.begin(); // Start the server
}

void loop() {
  // put your main code here, to run repeatedly:
  WiFiClient client = server.available();
  if(client){
      digitalWrite(RGB_BUILTIN, LOW);  // Turn the RGB LED white
    
  }
  else{
    digitalWrite(RGB_BUILTIN, HIGH);  // Turn the RGB LED white
  }
  
}
