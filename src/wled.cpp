#include "wled.h"

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#include "connectivity.h"
#include "state.h"

// WLED
#define PIN 15
#define NUMPIXELS 64 // 420

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

const auto pin_output = 16;
const auto pin_input = 4;
const auto bounce_color = pixels.Color(255, 0, 0);

auto last_value = LOW;
unsigned long last_debounce_time = 0;
unsigned long debounce_delay = 50;

void bounce_wled()
{
    for (int i = 0; i < (NUMPIXELS); i++)
    {
        if (connection_ready)
            return;
        pixels.clear();
        pixels.setPixelColor(i, bounce_color);
        pixels.show();
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
    for (int i = 0; i < (NUMPIXELS); i++)
    {
        if (connection_ready)
            return;
        pixels.clear();
        pixels.setPixelColor((NUMPIXELS - 1) - i, bounce_color);
        pixels.show();
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void show_wled_task(void *params)
{
    pinMode(pin_output, OUTPUT);
    pinMode(pin_input, INPUT_PULLDOWN);

    digitalWrite(pin_output, HIGH);
    last_debounce_time = millis();

    pixels.begin();
    pixels.setBrightness(50); // 255

    while (true)
    {
        if (!connection_ready)
        {
            bounce_wled();
        }
        else
        {
            auto reading = digitalRead(pin_input);

            if (reading != last_value)
            {
                last_debounce_time = millis();
            }

            if ((millis() - last_debounce_time) > debounce_delay)
            {
                if (reading != last_value)
                {
                    last_value = reading;
                }
            }

            auto color = get_color(reading);
            pixels.clear();
            pixels.setBrightness(get_brightness(reading));
            pixels.fill(color, 0, NUMPIXELS);
            pixels.show();
        }
    }
}
