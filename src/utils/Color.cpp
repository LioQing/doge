#include <doge/utils/Color.hpp>

namespace doge
{
    Color::Color(std::uint32_t value)
    {
        a = value % 0x100;
        b = value >> 8 % 0x100;
        g = value >> 16 % 0x100;
        r = value >> 24 % 0x100;
    }

    Color::Color(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a)
        : r(r), g(g), b(b), a(a)
    {
    }

    Color::operator std::uint32_t() const
    {
        return r << 24 | g << 16 | b << 8 | a;
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