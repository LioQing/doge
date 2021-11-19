#pragma once

#include <SFML/Graphics.hpp>
#include "Vec2.hpp"
#include "Rect.hpp"
#include "Color.hpp"

namespace doge
{
    struct cast
    {
        cast(const cast&) = delete;

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

        template <typename T>
        static Rect<T> FromSfRect(const sf::Rect<T>& r)
        {
            return Rect<T>(r.left, r.top, r.width, r.height);
        }

        template <typename T>
        static sf::Rect<T> ToSfRect(const Rect<T>& r)
        {
            return sf::Rect<T>(r.left, r.top, r.width, r.height);
        }

        static Color FromSfColor(const sf::Color& color);
        static sf::Color ToSfColor(const Color& color);

        static float ToDegree(float radian);
        static float ToRadian(float degree);
    };
}