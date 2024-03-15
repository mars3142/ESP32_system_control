#pragma once

#include <Arduino.h>
#include <mutex>

class Settings
{
public:
    void setPrimaryColor(u_int8_t color);
    u_int8_t getBackgroundColor();
    u_int8_t getForegroundColor();
    const u_int8_t *getPrimaryFont();
    const u_int8_t *getSecondaryFont();

private:
    u_int8_t m_background_color = 0;
};
