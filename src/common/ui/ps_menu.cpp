#include "common/ui/ps_menu.h"

#include "display.h"
#include "common/icons.h"
#include "common/menu_callback.h"
#include "common/settings.h"
#include "common/ui/scrollbar.h"

PSMenu::PSMenu()
{
}

void PSMenu::render(unsigned long dt)
{
    if (m_selected_item < 0)
    {
        pressed_down();
    }

    Settings settings;
    u_int8_t i = 1;
    auto display = getDisplay();
    display->setColorIndex(settings.getBackgroundColor());
    display->drawBox(0, 0, display->getDisplayWidth(), display->getDisplayHeight());

    display->setColorIndex(settings.getForegroundColor());

    drawScrollBar(dt);
    drawSelectionBox(dt);

    int x = 4 + 16 + 3; // sure?
    auto widget = m_items.at(m_selected_item);
    renderWidget(widget.getType(), u8g2_font_helvB08_tr, x, display->getDisplayHeight() / 2 + 3, widget.getText()->c_str());

    if (m_selected_item > 0)
    {
        auto widget = m_items.at(m_selected_item - 1);
        renderWidget(widget.getType(), u8g2_font_haxrcorp4089_tr, x, 14, widget.getText()->c_str());
    }
    if (m_selected_item < m_items.size() - 1)
    {
        auto widget = m_items.at(m_selected_item + 1);
        renderWidget(widget.getType(), u8g2_font_haxrcorp4089_tr, x, display->getDisplayHeight() - 10, widget.getText()->c_str());
    }
};

void PSMenu::renderWidget(const uint8_t type, const uint8_t *font, const int x, const int y, const char *text)
{
    auto display = getDisplay();
    switch (type)
    {
    case 0: // text
        display->setFont(font);
        display->drawStr(x, y, text);
        break;

    default:
        break;
    }
}

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
    m_items.at(m_selected_item).callback(m_selected_item);
}

void PSMenu::pressed_back()
{
    ///
}

void PSMenu::addText(const std::string &text, MenuCallback callback)
{
    m_items.push_back(MenuItem(0, text, callback));
}

void PSMenu::addSwitch(const std ::string &text, std::string &value, MenuCallback callback)
{
    m_items.push_back(MenuItem(1, text, value, callback));
}

void PSMenu::addNumber(const std::string &text, std::string &value, MenuCallback callback)
{
    m_items.push_back(MenuItem(2, text, value, callback));
}

void PSMenu::drawScrollBar(unsigned long dt)
{
    auto display = getDisplay();
    ScrollBar scrollBar(display->getDisplayWidth() - 3, 3, 1, display->getDisplayHeight() - 6);
    scrollBar.update(m_selected_item, m_items.size());
    scrollBar.render(dt);
}

void PSMenu::drawSelectionBox(unsigned long dt)
{
    auto display = getDisplay();
    int rightPadding = 8;
    display->drawRFrame(2, display->getDisplayHeight() / 3, display->getDisplayWidth() - rightPadding, display->getDisplayHeight() / 3, 3);
    int y = (display->getDisplayHeight() / 3) * 2 - 2;
    display->drawLine(4, y, display->getDisplayWidth() - rightPadding, y);
    int x = display->getDisplayWidth() - rightPadding;
    display->drawLine(x, y - display->getDisplayHeight() / 3 + 3, x, y - 1);
}
