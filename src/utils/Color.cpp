#include <doge/utils/Color.hpp>

namespace doge
{
    Color::Color(uint32_t value)
    {
        a = value % 0x100;
        b = value >> 8 % 0x100;
        g = value >> 16 % 0x100;
        r = value >> 24 % 0x100;
    }

    Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
        : r(r), g(g), b(b), a(a)
    {
    }

    const Color Color::Red          = 0xFF0000FF;
    const Color Color::Green        = 0x00FF00FF;
    const Color Color::Blue         = 0x0000FFFF;
    const Color Color::Yellow       = 0xFFFF00FF;
    const Color Color::Magenta      = 0xFF00FFFF;
    const Color Color::Cyan         = 0x00FFFFFF;
    const Color Color::White        = 0xFFFFFFFF;
    const Color Color::Black        = 0x000000FF;
    const Color Color::Transparent  = 0x00000000;
}