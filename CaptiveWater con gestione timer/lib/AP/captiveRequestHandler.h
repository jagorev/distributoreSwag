#ifndef CAPTIVEREQUESTHANDLER_H
#define CAPTIVEREQUESTHANDLER_H

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include "AP.h"

// managing the request for the captive portal
class CaptiveRequestHandler : public AsyncWebHandler
{
public:
    CaptiveRequestHandler();
    virtual ~CaptiveRequestHandler();

    bool canHandle(AsyncWebServerRequest *request) override;
    void handleRequest(AsyncWebServerRequest *request) override;
};

#endif
