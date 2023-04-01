#include "settings.h"

#include <Adafruit_NeoPixel.h>

const auto pixel = Adafruit_NeoPixel();
const auto day_color = pixel.Color(127, 127, 127);
const auto night_color = pixel.Color(0, 0, 255);

const char *get_values()
{
    return "{\"state\":\"day\", \"colorDay\":\"#00FF00\", \"brightnessDay\":10, \"colorNight\":\"#0000FF\", \"brightnessNight\":50, \"duration\":60}";
}

uint8_t get_brightness(int step = 0)
{
    if (step == 0)
        return 10;

    return 50;
}

uint32_t get_color(int step = 0)
{
    if (step == 0)
        return day_color;

    return night_color;
}
