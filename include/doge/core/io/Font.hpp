#pragma once

#include <SFML/Graphics.hpp>
#include <doge/utils/Rect.hpp>

namespace doge::io
{
    struct Font
    {
        struct Glyph
        {
            float advance;
            Rectf bounds;
            Recti texture_rectangle;
        };

        sf::Font font;

        bool FromFile(const std::string& filename);
        bool FromMemory(const void* data, std::size_t size);

        const std::string& GetFamily() const;
        Glyph GetGlyph(std::uint32_t code_point, std::uint32_t character_size, bool bold, float outline_thickness = 0.f) const;
        float GetKerning(std::uint32_t first, std::uint32_t second, std::uint32_t character_size) const;
        float GetLineSpacing(std::uint32_t character_size) const;
        float GetUnderlinePosition(std::uint32_t character_size) const;
        float GetUnderlineThickness(std::uint32_t character_size) const;
        float GetLetterSpacing(std::uint32_t first_code_point, std::uint32_t second_code_point, std::uint32_t character_size, bool bold, float outline_thickness = 0.f) const;
    };
}