#include "common/ui/ps_menu.h"

#include "common/icons.h"
#include "common/menu_callback.h"
#include "common/settings.h"
#include "common/ui/scrollbar.h"

PSMenu::PSMenu(U8G2 *display) : m_display(display)
{
}

void PSMenu::render(unsigned long dt)
{
    Settings settings;
    u_int8_t i = 1;
    m_display->setColorIndex(settings.getBackgroundColor());
    m_display->drawBox(0, 0, m_display->getDisplayWidth(), m_display->getDisplayHeight());

    m_display->setColorIndex(settings.getForegroundColor());

    drawScrollBar(dt);
    drawSelectionBox(dt);

    // auto icon = bitmap_led_1;

    int x = 4 + 16 + 3; // sure?
    m_display->setFont(u8g2_font_helvB08_tr);
    m_display->drawStr(x, m_display->getDisplayHeight() / 2 + 3, m_items.at(m_selected_item).getText()->c_str());
    // m_display->drawXBMP(6, m_display->getDisplayHeight() / 2 - 8, 14, 14, icon);

    m_display->setFont(u8g2_font_haxrcorp4089_tr);
    if (m_selected_item > 0)
    {
        m_display->drawStr(x, 14, m_items.at(m_selected_item - 1).getText()->c_str());
        // m_display->drawXBMP(6, 4, 14, 14, icon);
    }
    if (m_selected_item < m_items.size() - 1)
    {
        m_display->drawStr(x, m_display->getDisplayHeight() - 10, m_items.at(m_selected_item + 1).getText()->c_str());
        // m_display->drawXBMP(6, m_display->getDisplayHeight() - 19, 14, 14, icon);
    }
};

void PSMenu::onButtonClicked(uint8_t button)
{
    switch (button)
    {
    case PIN_UP:
        pressed_up();
        break;

    case PIN_DOWN:
        pressed_down();
        break;

    case PIN_LEFT:
        pressed_left();
        break;

    case PIN_RIGHT:
        pressed_right();
        break;

    case PIN_SELECT:
        pressed_select();
        break;

    case PIN_BACK:
        pressed_back();
        break;

    default:
        break;
    }
};

void PSMenu::pressed_down()
{
    if (m_selected_item == m_items.size() - 1)
    {
        m_selected_item = 0;
    }
    else
    {
        m_selected_item++;
    }
}

void PSMenu::pressed_up()
{
    if (m_selected_item == 0)
    {
        m_selected_item = m_items.size() - 1;
    }
    else
    {
        m_selected_item--;
    }
}

void PSMenu::pressed_left()
{
    ///
}

void PSMenu::pressed_right()
{
    ///
}

void PSMenu::pressed_select()
{
    ///
}

void PSMenu::pressed_back()
{
    ///
}

void PSMenu::addItem(const std::string &text, MenuCallback callback)
{
    m_items.push_back(MenuItem(text, callback));
}

void PSMenu::drawScrollBar(unsigned long dt)
{
    ScrollBar scrollBar(m_display, m_display->getDisplayWidth() - 3, 3, 1, m_display->getDisplayHeight() - 6);
    scrollBar.update(m_selected_item, m_items.size());
    scrollBar.render(dt);
}

void PSMenu::drawSelectionBox(unsigned long dt)
{
    int rightPadding = 8;
    m_display->drawRFrame(2, m_display->getDisplayHeight() / 3, m_display->getDisplayWidth() - rightPadding, m_display->getDisplayHeight() / 3, 3);
    int y = (m_display->getDisplayHeight() / 3) * 2 - 2;
    m_display->drawLine(4, y, m_display->getDisplayWidth() - rightPadding, y);
    int x = m_display->getDisplayWidth() - rightPadding;
    m_display->drawLine(x, y - m_display->getDisplayHeight() / 3 + 3, x, y - 1);
}
