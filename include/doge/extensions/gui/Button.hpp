#pragma once

#include <doge/utils/aliases.hpp>
#include <doge/extensions/gui/GUIElement.hpp>

namespace doge
{
    struct Button : GUIElement
    {
        Event<> on_pressed;
        Event<> on_released;
        Event<> on_mouse_entered;
        Event<> on_mouse_left;
        Event<> on_clicked;

        void Initialize(Engine& engine) override;

        bool IsDown() const;
        bool IsMouseOver() const;

    private:

        enum State
        {
            Down = 0,
            MouseOver = 1,
            Count
        };

        std::bitset<State::Count> states;
    };
}