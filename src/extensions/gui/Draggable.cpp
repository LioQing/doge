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
                is_dragging = true;
                on_drag_began(GetGUI().GetEngine().window.MapPixelToCoords(event.position, GetCameraComponent()));
            }
        });

        GetGUI().GetEngine().events.on_mouse_button_released.AddListener("doge_gui_draggable_" + GetID(),
        [&](const event::MouseButton& event)
        {
            if (event.button == event::MouseButton::Button::Left)
            {
                is_dragging = false;
                on_drag_ended(GetGUI().GetEngine().window.MapPixelToCoords(event.position, GetCameraComponent()));
            }
        });

        GetGUI().GetEngine().events.on_mouse_moved.AddListener("doge_gui_draggable_" + GetID(),
        [&](const event::MouseMove& event)
        {
            if (is_dragging)
            {
                on_dragged(GetGUI().GetEngine().window.MapPixelToCoords(event.position, GetCameraComponent()));
            }
        });

        GetEntity().GetComponent<doge::gui::Component>().OnRemoval([&]
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
}