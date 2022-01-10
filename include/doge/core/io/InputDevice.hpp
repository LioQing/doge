#pragma once

#include <SFML/Graphics.hpp>
#include <doge/utils/Vec2.hpp>
#include <memory>

namespace doge::io
{
    struct Window;

    struct InputDevice
    {
        struct Keyboard
        {
            using Key = sf::Keyboard::Key;

            Keyboard(const Keyboard&) = delete;

            static bool IsKeyDown(Key key);
            static void SetVirtualKeyboardVisible(bool visible);
        };

        struct Mouse
        {
            enum Button
            {
                Left, Right, Middle, Mouse4, Mouse5, 
                Count
            };

            enum Wheel
            {
                Vertical, Horizontal
            };

            Mouse(const Mouse&) = delete;

            static bool IsButtonDown(Button button);

            static void SetPosition(const Vec2i& position);
            static void SetPosition(const Vec2i& position, const Window& window);

            static Vec2i GetPosition();
            static Vec2i GetPosition(const Window& window);
        };

        struct Controller
        {
            
        };

        struct Touch
        {

        };

        struct Sensor
        {

        };

        std::weak_ptr<sf::RenderWindow> window_wptr;

        void SetRelativeMousePosition(const Vec2i& position) const;
        Vec2i GetRelativeMousePosition() const;
    };
}