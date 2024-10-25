#include "ui/main_menu.h"

#include "ui/light_menu.h"
#include "ui/settings_menu.h"

MainMenu::MainMenu() : PSMenu()
{
    addText("Lichtsteuerung", MainMenu::onSelect);
    addText("Einstellungen", MainMenu::onSelect);
}

void MainMenu::onSelect(uint8_t id)
{
    std::shared_ptr<Widget> widget;
    switch (id)
    {
    case 0:
        widget = std::make_shared<LightMenu>();
        break;
    case 1:
        widget = std::make_shared<SettingsMenu>();
        break;
    }
    pushScreen(widget);
}
