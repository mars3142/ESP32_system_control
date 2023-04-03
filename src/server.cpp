#include "server.h"

#include <ArduinoJson.h>
#include <AsyncElegantOTA.h>
#include <SPIFFS.h>

#include "state.h"
#include "website.h"

AsyncWebServer server(80);
AsyncWebSocket ws("/state");

void on_get_root(AsyncWebServerRequest *request)
{
    request->send(200, "text/html", index_html);
}

void on_post_state(AsyncWebServerRequest *request)
{
    request->send(200, "application/json", "{'status':true}");
}

void on_post_config(AsyncWebServerRequest *request)
{
    request->send(200, "application/json", "{'status':true}");
}

void on_ws_event(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
    auto json = DynamicJsonDocument(1024);
    auto err = deserializeJson(json, data);
    if (err && err != DeserializationError::EmptyInput)
    {
        log_e("deserializeJson() failed with code %s", err.c_str());
        return;
    }

    switch (type)
    {
    case WS_EVT_CONNECT:
        log_d("ws client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
        break;

    case WS_EVT_DISCONNECT:
        log_d("ws client #%u disconnect", client->id());
        break;

    case WS_EVT_DATA:
        server->textAll(get_values());
        break;

    case WS_EVT_PONG:
    case WS_EVT_ERROR:
        break;
    }
}

void init_server_task(void *param)
{
    log_d("Starting server init task");

    SPIFFS.begin(true);

    AsyncElegantOTA.begin(&server); // Start ElegantOTA

    ws.onEvent(on_ws_event);

    server.on("/", HTTP_GET, on_get_root);
    server.on("/state", HTTP_POST, on_post_state);
    server.on("/config", HTTP_POST, on_post_config);
    server.serveStatic("/", SPIFFS, "/");
    server.addHandler(&ws);
    server.begin();

    log_d("Server task init finished");
    vTaskDelete(NULL);
}
