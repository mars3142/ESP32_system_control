#pragma once

#include <string>

#include "common/menu_callback.h"

class MenuItem
{
public:
    MenuItem(const std::string &text, void (*callback)(int id));
    const std::string *getText() const;

private:
    std::string m_text;
    void (*m_callback)(int id);
};
