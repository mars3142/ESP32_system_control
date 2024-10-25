#include "ui/light_menu.h"

void demoL(uint8_t id)
{
    //
}

LightMenu::LightMenu()
{
    addText("Tag/Nacht", nullptr);
    addText("LED Einstellungen", demoL);
}
