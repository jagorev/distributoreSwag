#ifndef AP_H
#define AP_H

#include <DNSServer.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include "ESPAsyncWebServer.h"
#include <Arduino.h>

extern DNSServer dnsServer;
extern AsyncWebServer server;

enum AcquaSize
{
  cl25,
  cl33,
  cl50,
  l1
};

extern char *ssid;
extern char *password;
extern char index_html[] PROGMEM;
extern char landing_html[] PROGMEM;

// VARIABILI PER GESTIONE SCELTA ACQUA
extern bool scelta_effettuata;
extern AcquaSize acqua_scelta;


void refreshAP();
void setupServer();


#endif
