#pragma once

#include <doge/events.hpp>
#include <doge/utils.hpp>
#include <doge/core/io/Event.hpp>

namespace doge
{
    struct Events
    {
        Event<                          > on_window_opened;
        Event<                          > on_window_closed;
        Event<event::Size               > on_window_resized;
        Event<                          > on_window_focus_lost;
        Event<                          > on_window_focus_gained;
        Event<event::Text               > on_text_entered;
        Event<event::Key                > on_key_pressed;
        Event<event::Key                > on_key_released;
        Event<event::MouseWheel         > on_mouse_wheel_scrolled;
        Event<event::MouseButton        > on_mouse_button_pressed;
        Event<event::MouseButton        > on_mouse_button_released;
        Event<event::MouseMove          > on_mouse_moved;
        Event<                          > on_mouse_entered;
        Event<                          > on_mouse_left;
        Event<event::ControllerButton   > on_controller_button_pressed;
        Event<event::ControllerButton   > on_controller_button_released;
        Event<event::ControllerMove     > on_controller_moved;
        Event<event::ControllerConnect  > on_controller_connected;
        Event<event::ControllerConnect  > on_controller_disconnected;
        Event<event::Touch              > on_touch_began;
        Event<event::Touch              > on_touch_moved;
        Event<event::Touch              > on_touch_ended;
        Event<event::Sensor             > on_sensor_changed;

        io::Event event_io;

        Events();

        void OnIOEvent(const io::Event::Content& event);
    };
};