#include "common/ui/scrollbar.h"

#include "display.h"

ScrollBar::ScrollBar(int x, int y, int width, int height) : m_x(x), m_y(y), m_width(width), m_height(height), m_value(0), m_max(0), m_min(0)
{
}

void ScrollBar::render(unsigned long dt)
{
    if (1 == m_max)
        return;

    auto display = getDisplay();
    // draw dotted line
    for (int y = m_y; y < m_y + m_height; y++)
    {
        if (y % 2 == 0)
        {
            display->drawPixel(m_x, y);
        }
    }

    // draw thumb
    int thumbHeight = m_height / m_max;
    int thumbY = m_y + (m_value - m_min) * thumbHeight;
    display->drawBox(display->getWidth() - 4, thumbY, 3, thumbHeight);
}

void ScrollBar::update(int value, int max, int min)
{
    m_value = value;
    m_max = max;
    m_min = min;
}
