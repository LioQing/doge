#pragma once

#include <SFML/Graphics.hpp>
#include <doge/utils/Vec2.hpp>
#include <doge/utils/Vec3.hpp>
#include <memory>

namespace doge::io
{
    struct Window;

    struct Input
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

        struct Controller
        {
            struct Info
            {
                std::string name;
                std::uint32_t vendor_id;
                std::uint32_t product_id;
            };
            
            static constexpr std::uint8_t COUNT = 8;
            static constexpr std::uint8_t AXIS_COUNT = 8;
            static constexpr std::uint8_t BUTTON_COUNT = 32;

            Controller(const Controller&) = delete;

            static bool IsConnected(std::uint8_t id);

            static std::uint8_t GetButtonCount(std::uint8_t id);
            static bool HasAxis(std::uint8_t id, std::uint8_t axis);
            
            static bool IsButtonDown(std::uint8_t id, std::uint8_t button);
            static float GetAxisPosition(std::uint8_t id, std::uint8_t axis);

            static Info GetInfo(std::uint8_t id);
        };

        struct Touch
        {
            static bool IsDown(std::uint32_t finger);

            Touch(const Touch&) = delete;

            static Vec2i GetPosition(std::uint32_t finger);
            static Vec2i GetPosition(std::uint32_t finger, const Window& window);
        };

        struct Sensor
        {
            enum Type
            {
                Accelerometer,
                Gyroscope,
                Magnetometer,
                Gravity,
                UserAcceleration,
                Orientation,
                
                Count
            };

            Sensor(const Sensor&) = delete;

            static bool IsAvailable(Type sensor);
            static void SetEnabled(Type sensor, bool enabled);
            static Vec3f GetValue(Type sensor);
        };
    };
}