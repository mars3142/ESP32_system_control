#include "state.h"

#include <ArduinoJson.h>
#include <FastLED.h>

const auto day_color = CRGB(127, 127, 127);
const auto night_color = CRGB(0, 0, 255);

const char *get_values()
{
    char *result = new char[1024];
    DynamicJsonDocument doc(1024);

    doc["state"] = "day";
    doc["colorDay"] = "#00FF00";
    doc["colorNight"] = "#0000FF";
    doc["brightness"] = get_brightness();
    doc["delay"] = 2;

    serializeJson(doc, result, 1024);
    return result;
}

uint8_t get_brightness()
{
    return 192;
}
