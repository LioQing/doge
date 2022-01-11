#pragma once

#include <variant>
#include <SFML/Graphics.hpp>
#include <doge/events.hpp>
#include <doge/utils/aliases.hpp>
#include <doge/utils/lev.hpp>
#include <doge/utils/aliases.hpp>
#include <doge/core/io/Input.hpp>

namespace doge::io
{
    struct Event
    {
        enum Type
        {
            Closed,
            Resized,
            LostFocus,
            GainedFocus,
            TextEntered,
            KeyPressed,
            KeyReleased,
            MouseWheelScrolled,
            MouseButtonPressed,
            MouseButtonReleased,
            MouseMoved,
            MouseEntered,
            MouseLeft,
            ControllerButtonPressed,
            ControllerButtonReleased,
            ControllerMoved,
            ControllerConnected,
            ControllerDisconnected,
            TouchBegan,
            TouchMoved,
            TouchEnded,
            SensorChanged,
    
            Count
        };

        struct Content
        {
            Type type;

            std::variant<
                event::ControllerButton,
                event::ControllerConnect,
                event::ControllerMove,
                event::Key,
                event::MouseButton,
                event::MouseMove,
                event::MouseWheel,
                event::Sensor,
                event::Size,
                event::Text,
                event::Touch
            > event;
        };

        doge::Event<Content> event;

        void OnPollEvent(const sf::Event& event);
    };
}