#include "wled.h"

#include <Arduino.h>
#include <FastLED.h>
#include <esp32-hal-log.h>

#include "connectivity.h"
#include "state.h"

// WLED
#define DATA_PIN 15
#define NUM_LEDS 1024

CRGB leds[NUM_LEDS];

const auto pin_output = 16;
const auto pin_input = 4;
const auto bounce_color = CRGB::Magenta;
auto bounce_pixel = 0;
auto bounce_direction = 1;

auto last_value = LOW;
unsigned long last_debounce_time = 0;
unsigned long debounce_delay = 50;

void bounceWled()
{
    FastLED.clear();
    leds[bounce_pixel] = bounce_color;
    FastLED.show();
    vTaskDelay(100 / portTICK_PERIOD_MS);

    bounce_pixel += bounce_direction;
    if (bounce_pixel == getLedCount() - 1)
    {
        bounce_direction = -1;
    }
    else if (bounce_pixel == 0)
    {
        bounce_direction = 1;
    }
}

CRGBPalette16 pal;
uint8_t palette_index = 0;

bool day_night_direction = true;

TDynamicRGBGradientPalettePtr cur_pal = (TDynamicRGBGradientPalettePtr)malloc(2 * 4);

void showWledTask(void *params)
{
    FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    pinMode(pin_output, OUTPUT);
    pinMode(pin_input, INPUT_PULLDOWN);

    digitalWrite(pin_output, HIGH);
    last_debounce_time = millis();

    bool fade_color = true;

    while (true)
    {
        day_night_direction = true;
        FastLED.setBrightness(getBrightness());

        if (!connection_ready)
        {
            bounceWled();

            FastLED.clear();
            FastLED.show();
        }
        else if (fade_color)
        {
            auto grad_entry = (TRGBGradientPaletteEntryUnion *)cur_pal;
            grad_entry[0].index = 0;
            grad_entry[0].r = (day_night_direction) ? get_day_color().r : getNightColor().r;
            grad_entry[0].g = (day_night_direction) ? get_day_color().g : getNightColor().g;
            grad_entry[0].b = (day_night_direction) ? get_day_color().b : getNightColor().b;
            grad_entry[1].index = 255;
            grad_entry[1].r = (!day_night_direction) ? get_day_color().r : getNightColor().r;
            grad_entry[1].g = (!day_night_direction) ? get_day_color().g : getNightColor().g;
            grad_entry[1].b = (!day_night_direction) ? get_day_color().b : getNightColor().b;

            pal.loadDynamicGradientPalette(cur_pal);

            uint8_t indexCount = 255;
            auto totalTime = getDelay() * 1000.0f;
            auto startTime = millis();

            while (true)
            {
                auto progress = (millis() - startTime) / totalTime;
                uint8_t index = (uint8_t)(progress * (indexCount - 1));
                if (progress >= 1)
                {
                    index = indexCount - 1;
                }

                fill_solid(leds, getLedCount(), ColorFromPalette(pal, index, getBrightness(), TBlendType::LINEARBLEND_NOWRAP));
                FastLED.show();

                if (progress >= 1)
                {
                    break;
                }
            }

            fade_color = false;
        }
    }
}
