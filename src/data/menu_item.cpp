#include "data/menu_item.h"

MenuItem::MenuItem(const std::string &text, void (*callback)(int id)) : m_text(text), m_callback(callback)
{
}

const std::string *MenuItem::getText() const
{
    return &m_text;
}
