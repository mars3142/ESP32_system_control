#include "data/menu_item.h"

MenuItem::MenuItem(const uint8_t type, const std::string &text, void (*callback)(uint8_t id)) : m_type(type), m_text(text), m_value(""), m_callback(callback)
{
}

MenuItem::MenuItem(const uint8_t type, const std::string &text, std::string &value, void (*callback)(uint8_t id)) : m_type(type), m_text(text), m_value(value), m_callback(callback)
{
}

const uint8_t MenuItem::getType() const
{
    return m_type;
}

const std::string *MenuItem::getText() const
{
    return &m_text;
}

const std::string *MenuItem::getValue() const
{
    return &m_value;
}

const void MenuItem::setValue(const std::string &value)
{
    m_value = value;
}

const void MenuItem::callback(uint8_t id) const
{
    if (m_callback)
    {
        m_callback(id);
    }
    else
    {
        log_d("No callback for menu item %d", id);
    }
}

const bool MenuItem::hasCallback() const
{
    return m_callback;
}
