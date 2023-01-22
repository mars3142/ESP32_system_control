#pragma once
#include <Arduino.h>

struct RGB {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

RGB lerpRGB(RGB color1, RGB color2, unsigned long t) {
    RGB color;
    color.r = color1.r + ((color2.r - color1.r) * t);
    color.g = color1.g + ((color2.g - color1.g) * t);
    color.b = color1.b + ((color2.b - color1.b) * t);
    return color;
}
