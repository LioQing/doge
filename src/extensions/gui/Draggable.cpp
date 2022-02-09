#include <doge/extensions/gui/Draggable.hpp>

#include <doge/extensions/gui/GUI.hpp>

namespace doge::gui
{
    Draggable::~Draggable()
    {
    }

    void Draggable::Initialize()
    {
        GetGUI().GetEngine().events.on_mouse_button_pressed.AddListener("doge_gui_draggable_" + GetID(),
        [&](const event::MouseButton& event)
        {
            if (
                !is_dragging &&
                event.button == event::MouseButton::Button::Left && 
                GetGUI().GetElementBelowCursor().get() == static_cast<Element*>(this)
            )
            {
                mouse_start = MapPixelToCoordsChecked(event.position);

                is_dragging = true;
                on_drag_began(mouse_start);
            }
        });

        GetGUI().GetEngine().events.on_mouse_button_released.AddListener("doge_gui_draggable_" + GetID(),
        [&](const event::MouseButton& event)
        {
            if (event.button == event::MouseButton::Button::Left)
            {
                is_dragging = false;
                on_drag_ended(MapPixelToCoordsChecked(event.position));
            }
        });

        GetGUI().GetEngine().events.on_mouse_moved.AddListener("doge_gui_draggable_" + GetID(),
        [&](const event::MouseMove& event)
        {
            if (is_dragging)
            {
                auto pos = MapPixelToCoordsChecked(event.position);

                on_dragged(pos);
                on_dragged_diff(pos - mouse_start);
            }
        });

        GetEntity().GetComponent<ElementComponent>().OnRemoval([&]
        {
            GetGUI().GetEngine().events.on_mouse_button_pressed.RemoveListener("doge_gui_draggable_" + GetID());
            GetGUI().GetEngine().events.on_mouse_button_released.RemoveListener("doge_gui_draggable_" + GetID());
            GetGUI().GetEngine().events.on_mouse_moved.RemoveListener("doge_gui_draggable_" + GetID());
        });
    }

    bool Draggable::IsDragging() const
    {
        return is_dragging;
    }

    Vec2f Draggable::MapPixelToCoordsChecked(const Vec2i& pixel) const
    {
        auto coords = GetGUI().GetEngine().window.MapPixelToCoords(pixel, GetCameraComponent());

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