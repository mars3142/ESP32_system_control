#pragma once

#include "common/ui/widget.h"

class SplashScreen : virtual public Widget
{
public:
    void render(unsigned long dt) override;
};
