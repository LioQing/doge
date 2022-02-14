#pragma once

#include <SFML/Graphics.hpp>
#include <doge/utils/Vec2.hpp>

namespace doge::io
{
    struct Image;

    struct Cursor
    {
        using Type = sf::Cursor::Type;

        sf::Cursor cursor;

        bool FromFile(const std::string& filename, const Vec2u& hotspot = Vec2u::Zero);
        bool FromImage(const Image& image, const Vec2u& hotspot = Vec2u::Zero);
        bool FromPixels(const std::uint8_t* pixels, const Vec2u& size, const Vec2u& hotspot = Vec2u::Zero);
        bool FromSystem(Type type);
    };
}