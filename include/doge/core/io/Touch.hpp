#pragma once

#include <SFML/Graphics.hpp>
#include <doge/utils/Vec2.hpp>
#include <doge/utils/Vec3.hpp>

namespace doge::io
{
    struct Window;

    struct Touch
    {
        static bool IsDown(std::uint32_t finger);

        Touch(const Touch&) = delete;

        static Vec2i GetPosition(std::uint32_t finger);
        static Vec2i GetPosition(std::uint32_t finger, const Window& window);
    };
}