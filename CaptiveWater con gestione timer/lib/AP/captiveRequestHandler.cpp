#include "CaptiveRequestHandler.h"

CaptiveRequestHandler::CaptiveRequestHandler() {}

CaptiveRequestHandler::~CaptiveRequestHandler() {}

bool CaptiveRequestHandler::canHandle(AsyncWebServerRequest *request)
{
    return true;
}

void CaptiveRequestHandler::handleRequest(AsyncWebServerRequest *request)
{
    request->send_P(200, "text/html", index_html);
}
