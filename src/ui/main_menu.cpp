#include "ui/main_menu.h"

#include <FastLED.h>

MainMenu::MainMenu(U8G2 *display) : PSMenu(display)
{
    addItem("Lichtsteuerung", nullptr);
    addItem("Fahrstuhl", nullptr);
    addItem("Kamera-Zug", nullptr);
    addItem("Einstellungen", nullptr);
}
