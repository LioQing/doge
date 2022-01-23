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
            if (
                event.button == event::MouseButton::Button::Left && 
                math::TestPoint(engine.window.MapPixelToCoords(event.position, GetCameraComponent()), GetRectangle())
            )
            {
                on_pressed();
                states.set(State::Down, true);
            }
        };

        engine.events.on_mouse_button_released +=
        [&](const event::MouseButton& event)
        {
            if (
                event.button == event::MouseButton::Button::Left && 
                math::TestPoint(engine.window.MapPixelToCoords(event.position, GetCameraComponent()), GetRectangle())
            )
            {
                on_released();
                if (states.test(State::Down))
                    on_clicked();
            }

            states.set(State::Down, false);
        };

        engine.events.on_mouse_moved +=
        [&](const event::MouseMove& event)
        {
            if (
                !states.test(State::MouseOver) &&
                math::TestPoint(engine.window.MapPixelToCoords(event.position, GetCameraComponent()), GetRectangle())
            )
            {
                on_mouse_entered();
                states.set(State::MouseOver, true);
            }
            else if (
                states.test(State::MouseOver) &&
                !math::TestPoint(engine.window.MapPixelToCoords(event.position, GetCameraComponent()), GetRectangle())
            )
            {
                on_mouse_left();
                states.set(State::MouseOver, false);
            }
        };
    }

    bool Button::IsDown() const
    {
        return states.test(State::Down);
    }

    bool Button::IsMouseOver() const
    {
        return states.test(State::MouseOver);
    }
}