#pragma once

#include <Arduino.h>
#include <AsyncWebSocket.h>

void init_server_task(void *params);

void on_ws_event(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
