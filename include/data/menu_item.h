#pragma once

#include "common/menu_callback.h"

#include <string>

class MenuItem
{
public:
    MenuItem(const uint8_t type, const std::string &text, void (*callback)(uint8_t id));
    MenuItem(const uint8_t type, const std::string &text, std::string &value, void (*callback)(uint8_t id));
    const uint8_t getType() const;
    const std::string *getText() const;
    const std::string *getValue() const;
    const void setValue(const std::string &value);
    const void callback(uint8_t id) const;
    const bool hasCallback() const;

private:
    uint8_t m_type;
    std::string m_text;
    std::string m_value;
    void (*m_callback)(uint8_t id);
};
