#pragma once

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <doge/components/PolygonShape.hpp>
#include <doge/utils/Color.hpp>
#include <doge/utils/Vec2.hpp>
#include <doge/utils/Vec3.hpp>
#include <doge/utils/Rect.hpp>
#include <doge/core/WindowSettings.hpp>
#include <doge/core/io.hpp>
#include <doge/events.hpp>

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
        static Vec3<T> FromSfVec3(const sf::Vector3<T>& v)
        {
            return Vec3<T>(v.x, v.y, v.z);
        }

        template <typename T>
        static sf::Vector3<T> ToSfVec3(const Vec3<T>& v)
        {
            return sf::Vector3<T>(v.x, v.y, v.z);
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

        template <typename T>
        static Rect<T> RectFromVec2(const Vec2<T>& pos, const Vec2<T>& size)
        {
            return Rect<T>(pos.x, pos.y, size.x, size.y);
        }

        template <typename T>
        static Vec2<T> PosFromRect(const Rect<T>& r)
        {
            return Vec2<T>(r.left, r.top);
        }

        template <typename T>
        static Vec2<T> SizeFromRect(const Rect<T>& r)
        {
            return Vec2<T>(r.width, r.height);
        }

        template <typename T>
        static std::pair<Vec2<T>, Vec2<T>> FromRect(const Rect<T>& r)
        {
            return std::make_pair(Vec2<T>(r.left, r.top), Vec2<T>(r.width, r.height));
        }

        static Color FromSfColor(const sf::Color& color);
        static sf::Color ToSfColor(const Color& color);

        static float ToDegree(float radian);
        static float ToRadian(float degree);

        static WindowSettings::Style FromSfStyle(int style);
        static int ToSfStyle(WindowSettings::Style style);

        static io::Keyboard::Key FromSfKeyboardKey(sf::Keyboard::Key key);
        static sf::Keyboard::Key ToSfKeyboardKey(io::Keyboard::Key key);

        static io::Mouse::Wheel FromSfMouseWheel(sf::Mouse::Wheel wheel);
        static sf::Mouse::Wheel ToSfMouseWheel(io::Mouse::Wheel wheel);

        static io::Mouse::Button FromSfMouseButton(sf::Mouse::Button button);
        static sf::Mouse::Button ToSfMouseButton(io::Mouse::Button button);

        static io::Sensor::Type FromSfSensorType(sf::Sensor::Type type);
        static sf::Sensor::Type ToSfSensorType(io::Sensor::Type type);

        static io::Event::Content FromSfEvent(const sf::Event& event);

        static PolygonShape::Type FromSfPolygonType(sf::PrimitiveType type);
        static sf::PrimitiveType ToSfPolygonType(PolygonShape::Type type);

        static std::uint8_t FromSfControllerAxis(sf::Joystick::Axis axis);
        static sf::Joystick::Axis ToSfControllerAxis(std::uint8_t axis);
    };
}