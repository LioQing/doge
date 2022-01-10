#pragma once

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <doge/components/RigidBody.hpp>
#include <doge/utils/Color.hpp>
#include <doge/utils/Vec2.hpp>
#include <doge/utils/Rect.hpp>
#include <doge/core/WindowSettings.hpp>

namespace doge
{
    struct cast
    {
        cast(const cast&) = delete;

        static constexpr float ToB2Scale = .01f;

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

        static Vec2f FromB2Vec2(const b2Vec2& v);
        static b2Vec2 ToB2Vec2(const Vec2f& v);

        static float FromB2Length(float l);
        static float ToB2Length(float l);

        static b2BodyType ToB2BodyType(RigidBody::Type type);

        static float ToDegree(float radian);
        static float ToRadian(float degree);

        
        static WindowSettings::Style FromSfStyle(int style);
        static int ToSfStyle(WindowSettings::Style style);
    };
}