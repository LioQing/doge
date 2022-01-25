#pragma once

#include <string>
#include <map>
#include <doge/utils/Color.hpp>

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

        struct Appearance
        {
            Style style = Style::Regular;
            Color fill_color = Color::White;
            float outline_thickness = 0.f;
            Color outline_color = Color::Black;
        };

        std::string font_id = "";
        std::u32string string = U"";
        std::map<std::size_t, Appearance> character_appearance;
    };
}