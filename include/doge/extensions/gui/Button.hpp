#pragma once

#include <doge/utils/aliases.hpp>
#include <doge/extensions/gui/GUIElement.hpp>

namespace doge
{
    struct Button : GUIElement
    {
        Event<> on_button_pressed;
        Event<> on_button_released;
        Event<> on_button_hovered;

        void Initialize(Engine& engine) override;
    };
}