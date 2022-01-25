#pragma once

#include <SFML/Graphics.hpp>
#include <doge/utils/Vec2.hpp>
#include <doge/utils/Vec3.hpp>

namespace doge::io
{
    struct Controller
    {
        struct Info
        {
            const std::string& name;
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
}