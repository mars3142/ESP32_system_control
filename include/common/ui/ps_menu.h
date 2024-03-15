#pragma once

#include "common/ui/widget.h"
#include "common/menu_callback.h"
#include "data/menu_item.h"

#include <vector>
#include <U8G2lib.h>

typedef void (*MenuCallback)(int id);

class PSMenu : virtual public Widget
{
public:
    PSMenu(U8G2 *display);
    void render(unsigned long dt) override;
    void onButtonClicked(uint8_t button) override;
    void addItem(const std::string &text, MenuCallback callback);

private:
    void pressed_down();
    void pressed_up();
    void pressed_left();
    void pressed_right();
    void pressed_select();
    void pressed_back();

    void drawScrollBar(unsigned long dt);
    void drawSelectionBox(unsigned long dt);

    U8G2 *m_display;
    uint8_t m_selected_item = 0;
    std::vector<MenuItem> m_items;
};
