#include <doge/extensions/gui/Button.hpp>

#include <doge/core/Engine.hpp>
#include <doge/utils/math.hpp>

namespace doge
{
    void Button::Initialize(Engine& engine)
    {
        engine.events.on_mouse_button_pressed +=
        [&](const event::MouseButton& event)
        {
            if (event.button == event::MouseButton::Button::Left)
            {

            }
        };
    }
}