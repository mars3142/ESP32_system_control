#include <SPIFFS.h>
#include <AsyncElegantOTA.h>

#include "server.h"

#include "state.h"
#include "website.h"

AsyncWebServer server(80);
AsyncWebSocket ws("/state");

void init_server_task(void *param)
{
    log_d("Starting server init task");

    SPIFFS.begin(true);

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(200, "text/html", index_html); });

    server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(200, "application/json", "{\"state\":\"night\", \"colorDay\":\"#00FF00\", \"brightnessDay\":10, \"colorNight\":\"#0000FF\", \"brightnessNight\":50, \"duration\":60}"); });

    server.on("/state", HTTP_POST, [](AsyncWebServerRequest *request)
              {
    // change day/night cycle
    request->send(200, "application/json", "{'status':true}"); });

    server.on("/config", HTTP_POST, [](AsyncWebServerRequest *request)
              {
    // update settings
    request->send(200, "application/json", "{'success':true}"); });

    ws.onEvent(on_ws_event);
    server.addHandler(&ws);

    AsyncElegantOTA.begin(&server); // Start ElegantOTA

    server.begin();

    log_d("Server task init finished");
    vTaskDelete(NULL);
}

void on_ws_event(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
    switch (type)
    {
    case WS_EVT_CONNECT:
        log_d("ws[%s][%u] connect", server->url(), client->id());
        server->text(client->id(), get_state());
        break;

    case WS_EVT_DISCONNECT:
        log_d("ws[%s][%u] disconnect", server->url(), client->id());
        break;

    case WS_EVT_DATA:
        server->textAll(String(len) + " bytes received");
        break;

    case WS_EVT_PONG:
    case WS_EVT_ERROR:
        break;
    }
}
