#pragma once

#include <vector>

#include "common/ui/widget.h"
#include "common/menu_callback.h"
#include "data/menu_item.h"

typedef void (*MenuCallback)(uint8_t id);

class PSMenu : virtual public Widget
{
public:
    PSMenu();
    void render(unsigned long dt) override;
    void onButtonClicked(uint8_t button) override;
    void addText(const std::string &text, MenuCallback callback);
    void addSwitch(const std::string &text, std::string &value, MenuCallback callback);
    void addNumber(const std::string &text, std::string &value, MenuCallback callback);

private:
    void pressed_down();
    void pressed_up();
    void pressed_left();
    void pressed_right();
    void pressed_select();
    void pressed_back();

    void drawScrollBar(unsigned long dt);
    void drawSelectionBox(unsigned long dt);

    void renderWidget(const uint8_t type, const uint8_t *font, const int x, const int y, const char *text);

    int8_t m_selected_item = -1;
    std::vector<MenuItem> m_items;
};
