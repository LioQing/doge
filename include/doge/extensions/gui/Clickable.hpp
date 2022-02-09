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

        virtual void Initialize() override;

        virtual bool TestPoint(const Vec2f& point) const override;

        void SetCornerRadius(float corner_radius);
        float GetCornerRadius() const;
    
        bool IsDown(io::Mouse::Button button) const;
        bool IsMouseOver() const;

    private:

        bool is_mouse_over = false;
        float corner_radius = 0.f;
        std::bitset<io::Mouse::Button::Count> buttons_down;
    };
}