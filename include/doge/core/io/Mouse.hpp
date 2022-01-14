#pragma once

#include <SFML/Graphics.hpp>
#include <doge/utils/Vec2.hpp>
#include <doge/utils/Vec3.hpp>

namespace doge::io
{
    struct Window;

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
}