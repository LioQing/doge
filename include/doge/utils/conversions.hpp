#pragma once

#include <SFML/Graphics.hpp>
#include "Vec2.hpp"
#include "Color.hpp"

namespace doge
{
    struct conversions
    {
        conversions(const conversions&) = delete;

        template <typename T>
        static Vec2<T> FromSfVec2(const sf::Vector2<T>& v)
        {
            return Vec2<T>(v.x, v.y);
        }

        template <typename T>
        static sf::Vector2<T> ToSfVec2(const Vec2<T>& v)
        {
            return sf::Vector2<T>(v.x, v.y);
        }

        static Color FromSfColor(const sf::Color& color);
        static sf::Color ToSfColor(const Color& color);

        static float ToDegree(float radian);
        static float ToRadian(float degree);
    };
}