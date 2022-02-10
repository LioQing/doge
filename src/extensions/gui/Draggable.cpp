#include <doge/extensions/gui/Draggable.hpp>

#include <doge/extensions/gui/GUI.hpp>

namespace doge::gui
{
    Draggable::~Draggable()
    {
    }

    void Draggable::Initialize()
    {
    }

    bool Draggable::IsDragging() const
    {
        return is_dragging;
    }

    void Draggable::OnPressed(const Vec2f& position, io::Mouse::Button button)
    {
        mouse_start = CoordsChecked(position);

        if (!is_dragging && button == event::MouseButton::Button::Left)
        {
            is_dragging = true;
            GetGUI().SetElementBelowCursorLocked(true);
            on_drag_began(mouse_start);
        }
    }

    void Draggable::OnReleased(const Vec2f& position, io::Mouse::Button button)
    {
        if (button == event::MouseButton::Button::Left)
        {
            is_dragging = false;
            GetGUI().SetElementBelowCursorLocked(false);
            on_drag_ended(CoordsChecked(position));
        }
    }

    void Draggable::OnCursorMoved(const Vec2f& position)
    {
        if (is_dragging)
        {
            auto pos = CoordsChecked(position);

            on_dragged(pos);
            on_dragged_diff(pos - mouse_start);
        }
    }

    Vec2f Draggable::CoordsChecked(const Vec2f& coords_) const
    {
        Vec2f coords = coords_;

        if (coords.x > GetCameraComponent().size.x / 2.f - 1)
            coords.x = GetCameraComponent().size.x / 2.f - 1;
        else if (coords.x < -GetCameraComponent().size.x / 2.f + 1)
            coords.x = -GetCameraComponent().size.x / 2.f + 1;

        if (coords.y > GetCameraComponent().size.y / 2.f - 1)
            coords.y = GetCameraComponent().size.y / 2.f - 1;
        else if (coords.y < -GetCameraComponent().size.y / 2.f + 1)
            coords.y = -GetCameraComponent().size.y / 2.f + 1;
        
        return coords;
    }
}