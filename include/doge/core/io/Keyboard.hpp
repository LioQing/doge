#pragma once

#include <SFML/Graphics.hpp>
#include <doge/utils/Vec2.hpp>
#include <doge/utils/Vec3.hpp>

namespace doge::io
{
    struct Keyboard
    {
        using Key = sf::Keyboard::Key;

        Keyboard(const Keyboard&) = delete;

        static bool IsKeyDown(Key key);
        static void SetVirtualKeyboardVisible(bool visible);
    };
}