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

        static Color Red();
        static Color Green();
        static Color Blue();
        static Color Yellow();
        static Color Magenta();
        static Color Cyan();
        static Color White();
        static Color Black();
        static Color Transparent();
    };
}