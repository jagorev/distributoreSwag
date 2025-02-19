#include "CaptiveRequestHandler.h"

CaptiveRequestHandler::CaptiveRequestHandler() {}

CaptiveRequestHandler::~CaptiveRequestHandler() {}

// this function returns true for all the requests
bool CaptiveRequestHandler::canHandle(AsyncWebServerRequest *request)
{
    return true;
}

// this function is the one that actually sends what will be opened by the captive portal (i.e. index_html)
void CaptiveRequestHandler::handleRequest(AsyncWebServerRequest *request)
{
    request->send_P(200, "text/html", index_html);
}
