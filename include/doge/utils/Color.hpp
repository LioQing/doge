#pragma once

#include <cstdint>

namespace doge
{
    struct Color
    {
        uint8_t r = 0;
        uint8_t g = 0;
        uint8_t b = 0;
        uint8_t a = 0xFFu;

        Color() = default;
        Color(uint32_t value);
        Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xFFu);

        static const Color Red;
        static const Color Green;
        static const Color Blue;
        static const Color Yellow;
        static const Color Magenta;
        static const Color Cyan;
        static const Color White;
        static const Color Black;
        static const Color Transparent;
    };
}