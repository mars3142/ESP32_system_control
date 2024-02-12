#include "server.h"

#include <ArduinoJson.h>
#include <AsyncElegantOTA.h>
#include <SPIFFS.h>

#include "state.h"

AsyncWebServer server(80);
AsyncWebSocket ws("/state");

void onGetRoot(AsyncWebServerRequest *request)
{
    request->send(SPIFFS, "/index.html", "text/html");
}

void onPostState(AsyncWebServerRequest *request)
{
    request->send(200, "application/json", "{'status':true}");
}

void onPostConfig(AsyncWebServerRequest *request)
{
    request->send(200, "application/json", "{'status':true}");
}

void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
    auto json = DynamicJsonDocument(1024);
    auto err = deserializeJson(json, data);
    if (err && err != DeserializationError::EmptyInput)
    {
        log_e("deserializeJson() failed with code %s", err.c_str());
        return;
    }

    auto auth = json["auth"].as<String>();
    auto command = json["command"].as<String>();

    switch (type)
    {
    case WS_EVT_CONNECT:
        log_d("ws client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
        break;

    case WS_EVT_DISCONNECT:
        log_d("ws client #%u disconnect", client->id());
        break;

    case WS_EVT_DATA:
        if (auth != "eisenbahn")
        {
            log_e("Wrong auth");
            server->text(client->id(), "{\"error\":\"Wrong auth\"}");
            return;
        }
        log_d("ws client #%u command: %s", client->id(), command.c_str());
        server->textAll(getValues());
        break;

    case WS_EVT_PONG:
    case WS_EVT_ERROR:
        break;
    }
}

void initServerTask(void *param)
{
    log_d("Starting server init task");

    SPIFFS.begin(true);

    AsyncElegantOTA.begin(&server); // Start ElegantOTA

    ws.onEvent(onWsEvent);
    server.addHandler(&ws);

    server.on("/", HTTP_GET, onGetRoot);
    server.on("/state", HTTP_POST, onPostState);
    server.on("/config", HTTP_POST, onPostConfig);
    server.serveStatic("/", SPIFFS, "/");
    server.begin();

    log_d("Server task init finished");
    vTaskDelete(NULL);
}
