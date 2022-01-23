#pragma once

#include <doge/utils/aliases.hpp>

namespace doge
{
    struct Button
    {
        Event<> on_button_pressed;
        Event<> on_button_released;
        Event<> on_button_hovered;
    };
}