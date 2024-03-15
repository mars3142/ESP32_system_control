#include <Arduino.h>

#include <vector>

#include <U8g2lib.h>
#include <FastLED.h>
#include <OneButton.h>

#include "ui/main_menu.h"

// U8G2_SSD1309_128X64_NONAME2_1_HW_I2C display(U8G2_R0, /* reset */ U8X8_PIN_NONE, PIN_SCL, PIN_SDA);
U8G2_SH1106_128X64_NONAME_F_HW_I2C display(U8G2_R0, /* reset */ U8X8_PIN_NONE, PIN_SCL, PIN_SDA);
// U8G2_SSD1306_128X64_NONAME_1_HW_I2C display(U8G2_R0, /* reset */ U8X8_PIN_NONE, PIN_SCL, PIN_SDA);

Widget *widget = new MainMenu(&display);

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

  display.setI2CAddress(0x78); // 0x3c * 2
  display.begin();             // start the u8g2 library

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

  display.clearBuffer();
  widget->render(deltaTime);
  display.sendBuffer();
}
