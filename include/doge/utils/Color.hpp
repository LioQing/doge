#pragma once

#include <cstdint>

#if defined(WIN32) || defined(_WIN32)
    #define DLL_EXPORT __declspec(dllexport)
#else
    #define DLL_EXPORT
#endif

namespace doge
{
    struct Color
    {
        std::uint8_t r = 0;
        std::uint8_t g = 0;
        std::uint8_t b = 0;
        std::uint8_t a = 0xFFu;

        Color() = default;
        Color(std::uint32_t value);
        Color(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a = 0xFFu);

        operator std::uint32_t() const;

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