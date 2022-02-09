#include <doge/core/io/Font.hpp>

#include <doge/utils/cast.hpp>

namespace doge::io
{
    bool Font::FromFile(const std::string& filename)
    {
        return font.loadFromFile(filename);
    }

    bool Font::FromMemory(const void* data, std::size_t size)
    {
        return font.loadFromMemory(data, size);
    }

    const std::string& Font::GetFamily() const
    {
        return font.getInfo().family;
    }

    Font::Glyph Font::GetGlyph(std::uint32_t code_point, std::uint32_t character_size, bool bold, float outline_thickness) const
    {
        sf::Glyph glyph = font.getGlyph(code_point, character_size, bold, outline_thickness);
        return Glyph(glyph.advance, cast::FromSfRect(glyph.bounds), cast::FromSfRect(glyph.textureRect));
    }

    float Font::GetKerning(std::uint32_t first, std::uint32_t second, std::uint32_t character_size) const
    {
        return font.getKerning(first, second, character_size);
    }

    float Font::GetLineSpacing(std::uint32_t character_size) const
    {
        return font.getLineSpacing(character_size);
    }

    float Font::GetUnderlinePosition(std::uint32_t character_size) const
    {
        return font.getUnderlinePosition(character_size);
    }

    float Font::GetUnderlineThickness(std::uint32_t character_size) const
    {
        return font.getUnderlineThickness(character_size);
    }

    float Font::GetLetterSpacing(std::uint32_t first_code_point, std::uint32_t second_code_point, std::uint32_t character_size, bool bold, float outline_thickness) const
    {
        return GetGlyph(first_code_point, character_size, bold, outline_thickness).advance
            + GetKerning(first_code_point, second_code_point, character_size);
    }
}