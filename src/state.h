#pragma once

#include <FastLED.h>

const char *getValues();

void setBrightness(uint8_t brightness);

uint8_t getBrightness();

void setLedCount(uint32_t led_count);

uint32_t getLedCount();

void setDelay(uint32_t delay);

uint32_t getDelay(int multiplier = 1);

void setDayColor(String color);

CRGB get_day_color();

void setNightColor(String color);

CRGB getNightColor();
