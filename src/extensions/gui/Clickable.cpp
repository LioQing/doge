#include <doge/extensions/gui/Clickable.hpp>

#include <doge/extensions/gui/GUI.hpp>

namespace doge::gui
{
    Clickable::~Clickable()
    {
    }

    void Clickable::Initialize()
    {
        GetGUI().GetEngine().events.on_mouse_button_pressed.AddListener("doge_gui_clickable_" + GetID(),
        [&](const event::MouseButton& event)
        {
            if (GetGUI().GetElementBelowCursor().get() == static_cast<Element*>(this))
            {
                on_pressed(event.button);
                buttons_down.set(event.button, true);
            }
        });

        GetGUI().GetEngine().events.on_mouse_button_released.AddListener("doge_gui_clickable_" + GetID(),
        [&](const event::MouseButton& event)
        {
            if (buttons_down.any())
            {
                if (buttons_down.test(event.button))
                {
                    on_released(event.button);
                    if (GetGUI().GetElementBelowCursor().get() == static_cast<Element*>(this))
                        on_clicked(event.button);
                }

                buttons_down.set(event.button, false);
            }
        });

        GetGUI().GetEngine().events.on_mouse_moved.AddListener("doge_gui_clickable_" + GetID(),
        [&](const event::MouseMove& event)
        {
            if (
                !is_mouse_over &&
                GetGUI().GetElementBelowCursor().get() == static_cast<Element*>(this)
            )
            {
                on_mouse_entered();
                is_mouse_over = true;
            }
            else if (
                is_mouse_over &&
                GetGUI().GetElementBelowCursor().get() != static_cast<Element*>(this)
            )
            {
                on_mouse_left();
                is_mouse_over = false;
            }
        });

        GetEntity().GetComponent<doge::gui::Component>().OnRemoval([&]
        {
            GetGUI().GetEngine().events.on_mouse_button_pressed.RemoveListener("doge_gui_clickable_" + GetID());
            GetGUI().GetEngine().events.on_mouse_button_released.RemoveListener("doge_gui_clickable_" + GetID());
            GetGUI().GetEngine().events.on_mouse_moved.RemoveListener("doge_gui_clickable_" + GetID());
        });
    }
    
    bool Clickable::IsDown(io::Mouse::Button button) const
    {
        return buttons_down.test(button);
    }

    bool Clickable::IsMouseOver() const
    {
        return is_mouse_over;
    }
}