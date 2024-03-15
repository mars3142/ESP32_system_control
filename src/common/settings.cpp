#include "common/settings.h"

#include <U8g2lib.h>

u_int8_t Settings::getBackgroundColor()
{
    return m_background_color;
}

void Settings::setPrimaryColor(u_int8_t color)
{
}

u_int8_t Settings::getForegroundColor()
{
    return 1 - m_background_color;
}

const u_int8_t *Settings::getPrimaryFont()
{
    return u8g2_font_haxrcorp4089_tr;
}

const u_int8_t *Settings::getSecondaryFont()
{
    return u8g2_font_helvB08_tr;
}
