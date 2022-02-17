#pragma once

#include <SFML/Graphics.hpp>
#include <doge/components/CustomShape.hpp>
#include <doge/components/Text.hpp>
#include <doge/utils/Color.hpp>
#include <doge/utils/Vec2.hpp>
#include <doge/utils/Vec3.hpp>
#include <doge/utils/Rect.hpp>
#include <doge/core/WindowSettings.hpp>
#include <doge/core/io/Mouse.hpp>
#include <doge/core/io/Keyboard.hpp>
#include <doge/core/io/Sensor.hpp>
#include <doge/core/io/Event.hpp>
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

        static Color FromSfColor(const sf::Color& color);
        static sf::Color ToSfColor(const Color& color);

        static float ToDegree(float radian);
        static float ToRadian(float degree);

        static std::int32_t FromSfStyle(std::int32_t style);
        static std::int32_t ToSfStyle(std::int32_t style);

        static io::Keyboard::Key FromSfKeyboardKey(sf::Keyboard::Key key);
        static sf::Keyboard::Key ToSfKeyboardKey(io::Keyboard::Key key);

        static io::Mouse::Wheel FromSfMouseWheel(sf::Mouse::Wheel wheel);
        static sf::Mouse::Wheel ToSfMouseWheel(io::Mouse::Wheel wheel);

        static io::Mouse::Button FromSfMouseButton(sf::Mouse::Button button);
        static sf::Mouse::Button ToSfMouseButton(io::Mouse::Button button);

        static io::Sensor::Type FromSfSensorType(sf::Sensor::Type type);
        static sf::Sensor::Type ToSfSensorType(io::Sensor::Type type);

        static io::Event::Content FromSfEvent(const sf::Event& event);

        static CustomShape::Type FromSfPolygonType(sf::PrimitiveType type);
        static sf::PrimitiveType ToSfPolygonType(CustomShape::Type type);

        static std::uint8_t FromSfControllerAxis(sf::Joystick::Axis axis);
        static sf::Joystick::Axis ToSfControllerAxis(std::uint8_t axis);

        static std::int32_t FromSfTextStyle(std::int32_t style);
        static std::int32_t ToSfTextStyle(std::int32_t style);
    };
}