#include "display.h"

// U8G2_SSD1309_128X64_NONAME2_1_HW_I2C display(U8G2_R0, /* reset */ U8X8_PIN_NONE, PIN_SCL, PIN_SDA);
U8G2_SH1106_128X64_NONAME_F_HW_I2C display(U8G2_R0, /* reset */ U8X8_PIN_NONE, PIN_SCL, PIN_SDA);
// U8G2_SSD1306_128X64_NONAME_1_HW_I2C display(U8G2_R0, /* reset */ U8X8_PIN_NONE, PIN_SCL, PIN_SDA);

U8G2 *getDisplay()
{
    return &display;
}
