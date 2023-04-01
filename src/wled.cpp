#include "wled.h"

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#include "connectivity.h"

// WLED
#define PIN 15
#define NUMPIXELS 420

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

const auto pinOutput = 16;
const auto pinInput = 4;
const auto dayColor = pixels.Color(127, 127, 127);
const auto nightColor = pixels.Color(0, 0, 255);

auto color = dayColor;
auto lastValue = LOW;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

void show_wled_task(void *params)
{
    pinMode(pinOutput, OUTPUT);
    pinMode(pinInput, INPUT_PULLDOWN);

    digitalWrite(pinOutput, HIGH);
    lastDebounceTime = millis();

    pixels.begin();
    pixels.setBrightness(50); // 255

    while (true)
    {
        if (!connection_ready)
        {
            for (int i = 0; i < 8; i++)
            {
                if (connection_ready)
                {
                    break;
                }
                pixels.clear();
                pixels.setPixelColor(i, pixels.Color(255, 0, 0));
                pixels.show();
                vTaskDelay(100 / portTICK_PERIOD_MS);
            }
        }
        else
        {
            auto reading = digitalRead(pinInput);

            if (reading != lastValue)
            {
                lastDebounceTime = millis();
            }

            if ((millis() - lastDebounceTime) > debounceDelay)
            {
                if (reading != lastValue)
                {
                    lastValue = reading;
                }
            }

            pixels.clear();
            pixels.fill((reading == LOW) ? dayColor : nightColor, 0, NUMPIXELS);
            pixels.show();
        }
    }
}
