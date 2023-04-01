#include "connectivity.h"

#include <Arduino.h>
#include <esp32-hal-log.h>
#include <WiFi.h>
#include <ESPmDNS.h>

#include "credentials.h"

bool init_mdns_done = false;
bool connection_ready = false;

void init_wifi_task(void *params)
{
    log_d("Starting init wifi task");

    WiFi.disconnect();
    if (WiFi.status() != WL_CONNECTED)
    {
        for (auto &credential : credentials)
        {
            WiFi.mode(WIFI_STA);
            WiFi.begin(credential.ssid.c_str(), credential.password.c_str());
            log_d("Trying to connect to %s", credential.ssid.c_str());

            // Wait for connection
            unsigned long start = millis();
            while (millis() - start <= 15000)
            {
                if (WiFi.status() == WL_CONNECTED)
                {
                    break;
                }

                delay(500);
            }
        }
        if (WiFi.status() != WL_CONNECTED)
        {
            log_w("Restarting because of no wifi connection");
            delay(2000);
            ESP.restart();
        }
    }

    if (!init_mdns_done)
    {
        init_mdns_done = MDNS.begin("eisenbahn");
    }

    if (!init_mdns_done)
    {
        log_d("Error setting up MDNS responder!");
        vTaskDelay(10000 / portTICK_PERIOD_MS);
        ESP.restart();
    }

    connection_ready = true;

    log_d("Wifi task init finished");
    vTaskDelete(NULL);
}
