#include "common/ui/scrollbar.h"

ScrollBar::ScrollBar(U8G2 *display, int x, int y, int width, int height) : m_display(display), m_x(x), m_y(y), m_width(width), m_height(height), m_value(0), m_max(0), m_min(0)
{
}

void ScrollBar::render(unsigned long dt)
{
    // draw dotted line
    for (int y = m_y; y < m_y + m_height; y++)
    {
        if (y % 2 == 0)
        {
            m_display->drawPixel(m_x, y);
        }
    }

    // draw thumb
    int thumbHeight = m_height / m_max;
    int thumbY = m_y + (m_value - m_min) * thumbHeight;
    m_display->drawBox(m_display->getWidth() - 4, thumbY, 3, thumbHeight);
}

void ScrollBar::update(int value, int max, int min)
{
    m_value = value;
    m_max = max;
    m_min = min;
}
