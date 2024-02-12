#include "state.h"

#include <ArduinoJson.h>
#include <Preferences.h>

Preferences preferences;

CRGB _colorFromString(const char *color)
{
    uint8_t red = 0x00;
    uint8_t green = 0x00;
    uint8_t blue = 0x00;

    sscanf(color, "#%02x%02x%02x", &red, &green, &blue);
    log_d("hex: %s -> red: %d, green: %d, blue: %d", color, red, green, blue);

    return CRGB(red, green, blue);
}

void _openPreferences()
{
    preferences.begin("wled", false);
}

void _closePreferences()
{
    preferences.end();
}

auto day_color = "#00FF00";
auto night_color = "#0000FF";
const char *getValues()
{
    char *result = new char[1024];
    DynamicJsonDocument doc(1024);

    _openPreferences();

    doc["state"] = preferences.getString("state", "day");
    doc["colorDay"] = preferences.getString("colorDay", day_color);
    doc["colorNight"] = preferences.getString("colorNight", night_color);
    doc["brightness"] = getBrightness();
    doc["delay"] = getDelay();
    doc["ledCount"] = getLedCount();

    _closePreferences();

    serializeJson(doc, result, 1024);
    return result;
}

void setBrightness(uint8_t brightness)
{
    _openPreferences();
    preferences.putUInt("brightness", brightness);
    _closePreferences();
}

uint8_t getBrightness()
{
    _openPreferences();
    auto brightness = preferences.getUInt("brightness", 192);
    _closePreferences();

    return brightness;
}

void setLedCount(uint32_t led_count)
{
    _openPreferences();
    preferences.putUInt("ledCount", led_count);
    _closePreferences();
}

uint32_t getLedCount()
{
    _openPreferences();
    auto led_count = preferences.getUInt("ledCount", 500);
    _closePreferences();

    return led_count;
}

uint32_t getDelay(int multiplier)
{
    _openPreferences();
    auto delay = preferences.getUInt("delay", 2) * multiplier;
    _closePreferences();

    return delay;
}

void setDelay(uint32_t delay)
{
    _openPreferences();
    preferences.putUInt("delay", delay);
    _closePreferences();
}

void setDayColor(String color)
{
    _openPreferences();
    preferences.putString("colorDay", color);
    _closePreferences();
}

CRGB get_day_color()
{
    _openPreferences();
    auto color = preferences.getString("colorDay", day_color);
    _closePreferences();
    return _colorFromString(color.c_str());
}

void setNightColor(String color)
{
    _openPreferences();
    preferences.putString("colorNight", color);
    _closePreferences();
}

CRGB getNightColor()
{
    _openPreferences();
    auto color = preferences.getString("colorNight", night_color);
    _closePreferences();
    return _colorFromString(color.c_str());
}
