#include "ui/light_menu.h"

void demoL(uint8_t id)
{
    //
}

LightMenu::LightMenu()
{
    addText("Umschalten", nullptr);
    addText("LED Einstellungen", demoL);
}
