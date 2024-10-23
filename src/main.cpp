#include <Arduino.h>

#include <vector>

#include <FastLED.h>
#include <OneButton.h>

#include "display.h"
#include "ui/main_menu.h"

Widget *widget = new MainMenu();

const uint8_t pins[] = {PIN_DOWN, PIN_UP, PIN_LEFT, PIN_RIGHT, PIN_SELECT, PIN_BACK};
std::vector<OneButton> buttons;

unsigned long lastDeltaTime = 0;

void onMenuItemClicked(int id)
{
  /// TBD
}

void onMenuItemLongPressed(int id)
{
}

void onMenuItemReleased(int id)
{
}

void setScreen(Widget *screen)
{
  if (screen != nullptr)
  {
    if (widget != nullptr && widget != screen)
    {
      delete widget;
    }

    widget = screen;
  }
}

void onButtonClicked(void *payload)
{
  uint8_t *pin = (uint8_t *)payload;
  widget->onButtonClicked(*pin);
}

void setupButtons()
{
  for (auto pin : pins)
  {
    OneButton button(pin);
    button.attachClick(onButtonClicked, new uint8_t(pin));
    buttons.push_back(button);
  }
}

void setup()
{
  Serial.begin(115200);

  // 0x3c * 2
  getDisplay()->setI2CAddress(0x78);
  // start the u8g2 library
  getDisplay()->begin();

  setupButtons();
}

void loop()
{
  unsigned long deltaTime = millis() - lastDeltaTime;
  lastDeltaTime = deltaTime;

  for (auto &button : buttons)
  {
    button.tick();
  }

  getDisplay()->clearBuffer();
  widget->render(deltaTime);
  getDisplay()->sendBuffer();
}
