#pragma once

#include "data/menu_item.h"
#include "common/ui/ps_menu.h"

#include <U8g2lib.h>
#include <vector>

class MainMenu : public PSMenu
{
public:
    MainMenu();

private:
    static void onSelect(uint8_t id);
};
