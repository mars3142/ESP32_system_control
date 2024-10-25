#pragma once

#include <memory>

#include "common/ui/widget.h"

void pushScreen(std::shared_ptr<Widget> screen);
void popScreen();
