#pragma once

#include <doge/extensions/gui/CursorDetectableElement.hpp>
#include <doge/core/io/Mouse.hpp>

namespace doge::gui
{
    struct Clickable : CursorDetectableElement
    {
        Event<Vec2f, io::Mouse::Button> on_clicked;

        virtual ~Clickable();

        virtual void Initialize() override;

        virtual bool TestPoint(const Vec2f& point) const override;

        void SetCornerRadius(float corner_radius);
        float GetCornerRadius() const;
    
        bool IsDown(io::Mouse::Button button) const;
        bool IsMouseOver() const;
    
    protected:

        virtual void OnPressed(const Vec2f& position, io::Mouse::Button button) override;
        virtual void OnReleased(const Vec2f& position, io::Mouse::Button button) override;
        virtual void OnCursorLeft(const Vec2f& position) override;

    private:

        float corner_radius = 0.f;
        std::bitset<io::Mouse::Button::Count> buttons_down;
    };
}