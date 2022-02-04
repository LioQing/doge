#pragma once

#include <cstdint>

namespace doge::gui
{
    enum Align : std::uint8_t
    {
        Center = 0,

        Left = 0b01 << 0,
        Right = 0b10 << 0,

        Top = 0b01 << 2,
        Bottom = 0b10 << 2,
    };
}