#pragma once

#include <OneButton.h>

class Widget
{
public:
    virtual void render(unsigned long dt){};
    virtual void onButtonClicked(uint8_t button){};
};
