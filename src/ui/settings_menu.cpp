#include "ui/settings_menu.h"

#include "display.h"

void demo(uint8_t id)
{
    ///
}

SettingsMenu::SettingsMenu()
{
    addText("OTA Einspielen", demo);
}
