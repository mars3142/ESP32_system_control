#include "ui/main_menu.h"

#include "ui/light_menu.h"
#include "ui/settings_menu.h"

#include <FastLED.h>

MainMenu::MainMenu() : PSMenu()
{
    addText("Lichtsteuerung", MainMenu::onSelect);
    addText("Einstellungen", MainMenu::onSelect);
}

void MainMenu::onSelect(uint8_t id)
{
    Widget *widget = nullptr;
    switch (id)
    {
    case 0:
        widget = new LightMenu();
        break;
    case 1:
        widget = new SettingsMenu();
        break;
    }
    setScreen(widget);
}
