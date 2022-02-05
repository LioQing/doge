#pragma once

#include <string>
#include <map>
#include <doge/utils/Color.hpp>
#include <doge/utils/Vec2.hpp>

namespace doge
{
    struct Text
    {
        enum Style
        {
            Regular = 0,
            Bold = 1 << 0,
            Italic = 1 << 1,
            Underlined = 1 << 2,
            StrikeThrough = 1 << 3,
        };

        enum Align
        {
            Left,
            Center,
            Right,
        };

        struct Appearance
        {
            Style style = Style::Regular;
            Color fill_color = Color::White;
            float outline_thickness = 0.f;
            Color outline_color = Color::Black;
            float letter_spacing_factor = 1.f;
        };

        std::string font_id = "open_sans";
        std::u32string string = U"";
        std::uint32_t font_size = 18;
        float line_spacing_factor = 1.f;
        Align align = Align::Left;
        Vec2f origin = Vec2f::Zero;
        std::map<std::size_t, Appearance> character_appearances = { std::pair<std::size_t, Appearance>(0, Appearance()) };
    };
}