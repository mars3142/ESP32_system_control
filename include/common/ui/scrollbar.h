#pragma once

#include "common/ui/widget.h"

#include <U8g2lib.h>

class ScrollBar : virtual public Widget
{
public:
    ScrollBar(U8G2 *display, int x, int y, int width, int height);
    void render(unsigned long dt) override;
    void update(int value, int max, int min = 0);

private:
    int m_x;
    int m_y;
    int m_width;
    int m_height;
    int m_value;
    int m_max;
    int m_min;
    U8G2 *m_display;
};
