#include "wled.h"

#include <Arduino.h>
#include <FastLED.h>

#include "connectivity.h"
#include "state.h"

// WLED
#define DATA_PIN 15
#define NUM_PIXELS 64

CRGB strip[NUM_PIXELS];

const auto pin_output = 16;
const auto pin_input = 4;
const auto bounce_color = CRGB::Magenta;
auto bounce_pixel = 0;
auto bounce_direction = 1;

auto last_value = LOW;
unsigned long last_debounce_time = 0;
unsigned long debounce_delay = 50;

void bounce_wled()
{
    FastLED.clear();
    strip[bounce_pixel] = bounce_color;
    FastLED.show();
    vTaskDelay(100 / portTICK_PERIOD_MS);

    bounce_pixel += bounce_direction;
    if (bounce_pixel == NUM_PIXELS - 1)
    {
        bounce_direction = -1;
    }
    else if (bounce_pixel == 0)
    {
        bounce_direction = 1;
    }
}

CRGBPalette16 palette;
uint8_t palette_index = 0;

byte day_night[8] = {0, 255, 0, 0, 255, 0, 0, 255};

void show_wled_task(void *params)
{
    palette.loadDynamicGradientPalette(day_night);

    FastLED.addLeds<WS2812B, DATA_PIN, GRB>(strip, NUM_PIXELS);
    pinMode(pin_output, OUTPUT);
    pinMode(pin_input, INPUT_PULLDOWN);

    digitalWrite(pin_output, HIGH);
    last_debounce_time = millis();

    while (true)
    {
        FastLED.setBrightness(get_brightness());

        if (!connection_ready)
        {
            bounce_wled();
        }
        else
        {
            if (palette_index < 255)
                fill_solid(strip, NUM_PIXELS, ColorFromPalette(palette, palette_index++ % 255, get_brightness(), TBlendType::LINEARBLEND_NOWRAP));
            else
                fill_solid(strip, NUM_PIXELS, ColorFromPalette(palette, palette_index, get_brightness(), TBlendType::LINEARBLEND_NOWRAP));
            FastLED.show();
            /*
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
            */
        }
    }
}
