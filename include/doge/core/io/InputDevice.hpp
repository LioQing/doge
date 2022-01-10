#pragma once

#include <SFML/Graphics.hpp>

namespace doge::io
{
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
                Size
            };

            enum Wheel
            {
                Vertical, Horizontal
            };

            Mouse(const Mouse&) = delete;

            static bool IsButtonDown(Button button);
        };
    };
}