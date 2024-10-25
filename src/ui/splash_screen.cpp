#include "ui/splash_screen.h"

#include "common/settings.h"
#include "display.h"

void SplashScreen::render(unsigned long dt)
{
    Settings s;

    auto display = getDisplay();
    display->setFont(u8g2_font_DigitalDisco_tf);
    display->drawStr(28, display->getHeight() / 2 - 5, "HO Anlage");
    display->setFont(s.getPrimaryFont());
    display->drawStr(28, 40, "Initialisierung...");
}
