#include <doge/core/Events.hpp>

#include <functional>

namespace doge
{
    Events::Events()
    {
        event_io.event += std::bind(&Events::OnIOEvent, this, std::placeholders::_1);
    }

    void Events::OnIOEvent(const io::Event::Content& event)
    {
        using t = io::Event::Type;

        switch (event.type)
        {
            case t::Closed:                     on_window_closed                (                                               ); return;
            case t::Resized:                    on_window_resized               (std::get<event::Size             >(event.event)); return;
            case t::LostFocus:                  on_window_focus_lost            (                                               ); return;
            case t::GainedFocus:                on_window_focus_gained          (                                               ); return;
            case t::TextEntered:                on_text_entered                 (std::get<event::Text             >(event.event)); return;
            case t::KeyPressed:                 on_key_pressed                  (std::get<event::Key              >(event.event)); return;
            case t::KeyReleased:                on_key_released                 (std::get<event::Key              >(event.event)); return;
            case t::MouseWheelScrolled:         on_mouse_wheel_scrolled         (std::get<event::MouseWheel       >(event.event)); return;
            case t::MouseButtonPressed:         on_mouse_button_pressed         (std::get<event::MouseButton      >(event.event)); return;
            case t::MouseButtonReleased:        on_mouse_button_released        (std::get<event::MouseButton      >(event.event)); return;
            case t::MouseMoved:                 on_mouse_moved                  (std::get<event::MouseMove        >(event.event)); return;
            case t::MouseEntered:               on_mouse_entered                (                                               ); return;
            case t::MouseLeft:                  on_mouse_left                   (                                               ); return;
            case t::ControllerButtonPressed:    on_controller_button_pressed    (std::get<event::ControllerButton >(event.event)); return;
            case t::ControllerButtonReleased:   on_controller_button_released   (std::get<event::ControllerButton >(event.event)); return;
            case t::ControllerMoved:            on_controller_moved             (std::get<event::ControllerMove   >(event.event)); return;
            case t::ControllerConnected:        on_controller_connected         (std::get<event::ControllerConnect>(event.event)); return;
            case t::ControllerDisconnected:     on_controller_disconnected      (std::get<event::ControllerConnect>(event.event)); return;
            case t::TouchBegan:                 on_touch_began                  (std::get<event::Touch            >(event.event)); return;
            case t::TouchMoved:                 on_touch_moved                  (std::get<event::Touch            >(event.event)); return;
            case t::TouchEnded:                 on_touch_ended                  (std::get<event::Touch            >(event.event)); return;
            case t::SensorChanged:              on_sensor_changed               (std::get<event::Sensor           >(event.event)); return;

            default: throw std::invalid_argument("io::Event::Type not found"); return;
        }
    }
}