#include <Arduino.h>

#include <memory>
#include <vector>

#include <OneButton.h>

#include "display.h"
#include "ui/splash_screen.h"

std::shared_ptr<Widget> widget;
std::vector<std::shared_ptr<Widget>> history;

const uint8_t pins[] = {PIN_DOWN, PIN_UP, PIN_LEFT, PIN_RIGHT, PIN_SELECT, PIN_BACK};
std::vector<OneButton> buttons;

unsigned long lastDeltaTime = 0;

void popScreen()
{
  log_d("History count: %i", history.size());

  if (history.size() >= 2)
  {
    history.pop_back();
    widget = history.back();
  }
}

void pushScreen(std::shared_ptr<Widget> screen)
{
  log_d();

  if (screen != nullptr)
  {
    widget = screen;
    history.push_back(widget);
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
  widget = std::make_shared<SplashScreen>();
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
