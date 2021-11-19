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

    Color Color::Red()          { return Color(0xFF, 0, 0); }
    Color Color::Green()        { return Color(0, 0xFF, 0); }
    Color Color::Blue()         { return Color(0, 0, 0xFF); }
    Color Color::Yellow()       { return Color(0xFF, 0xFF, 0); }
    Color Color::Magenta()      { return Color(0xFF, 0, 0xFF); }
    Color Color::Cyan()         { return Color(0, 0xFF, 0xFF); }
    Color Color::White()        { return Color(0xFF, 0xFF, 0xFF); }
    Color Color::Black()        { return Color(0, 0, 0); }
    Color Color::Transparent()  { return Color(0, 0, 0, 0); }
}