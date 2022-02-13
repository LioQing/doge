#include <doge/extensions/gui/Clickable.hpp>

#include <doge/extensions/gui/GUI.hpp>

namespace doge::gui
{
    Clickable::~Clickable()
    {
    }

    void Clickable::Initialize()
    {
    }

    bool Clickable::TestPoint(const Vec2f& point) const
    {
        return math::TestPoint(point, GetGlobalRectangle(), corner_radius);
    }

    void Clickable::SetCornerRadius(float corner_radius)
    {
        this->corner_radius = corner_radius;
    }

    float Clickable::GetCornerRadius() const
    {
        return corner_radius;
    }
    
    bool Clickable::IsDown(io::Mouse::Button button) const
    {
        return buttons_down.test(button);
    }

    bool Clickable::IsMouseOver() const
    {
        return GetGUI().GetElementBelowCursor().get() == static_cast<const Element*>(this);
    }

    void Clickable::OnPressed(const Vec2f& position, io::Mouse::Button button)
    {
        buttons_down.set(button, true);
    }

    void Clickable::OnReleased(const Vec2f& position, io::Mouse::Button button)
    {
        auto was_down = buttons_down.test(button);
        buttons_down.set(button, false);
        
        if (was_down)
        {
            on_clicked(position, button);
        }
    }

    void Clickable::OnCursorLeft(const Vec2f& position)
    {
        buttons_down.reset();
    }
}