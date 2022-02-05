#pragma once

#include <doge/extensions/gui/Element.hpp>
#include <doge/core/io/Mouse.hpp>

namespace doge::gui
{
    struct Clickable : Element
    {
        Event<io::Mouse::Button> on_pressed;
        Event<io::Mouse::Button> on_released;
        Event<> on_mouse_entered;
        Event<> on_mouse_left;
        Event<io::Mouse::Button> on_clicked;

        virtual ~Clickable();

        virtual void Initialize();
    
        bool IsDown(io::Mouse::Button button) const;
        bool IsMouseOver() const;

    private:

        bool is_mouse_over = false;
        std::bitset<io::Mouse::Button::Count> buttons_down;
    };
}