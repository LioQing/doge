#include <doge/utils/custom_sf/Text.hpp>

#include <doge/components/Text.hpp>
#include <doge/utils/cast.hpp>
#include <doge/core/io/Font.hpp>
#include <doge/core/Assets.hpp>
#include <algorithm>

namespace doge::custom_sf
{
    Text::Text(const Assets& assets, const doge::Text& text_component)
    {
        Update(assets, text_component);
    }

    void Text::Update(const Assets& assets, const doge::Text& text_comp)
    {
        auto GetOffset = [&](std::size_t index, const doge::Text::Appearance& appear)
        {
            Vec2f offset;
            const io::Font& font = assets.fonts.at(text_comp.font_id);

            std::size_t line = 0;
            std::size_t line_start_index = 0;
            for (std::size_t i = 0; i < index; ++i)
            {
                if (text_comp.string.at(i) == '\n')
                {
                    ++line;
                    line_start_index = i + 1;
                    offset.x = 0.f;
                }
                else
                {
                    offset.x += (font.GetGlyph(
                        text_comp.string.at(i), 
                        text_comp.font_size, 
                        appear.style & doge::Text::Style::Bold, 
                        appear.outline_thickness).advance
                        + font.GetKerning(text_comp.string.at(i), text_comp.string.at(i + 1), text_comp.font_size))
                        * appear.letter_spacing_factor;
                }
            }

            offset.y = line * font.GetLineSpacing(text_comp.font_size) * text_comp.line_spacing_factor;

            return offset;
        };

        auto UpdateText = [&](sf::Text& text, std::size_t start, std::size_t end, const doge::Text::Appearance& appear)
        {
            text.setPosition(getPosition());
            text.setScale(getScale());
            text.setRotation(getRotation());
            text.setOrigin(cast::ToSfVec2(cast::FromSfVec2(getOrigin()) - GetOffset(start, appear)));
            
            sf::String string;
            for (std::size_t i = start; i < end; ++i)
            {
                string += sf::Uint32(text_comp.string.at(i));
            }
            text.setString(string);
            text.setFont(assets.fonts.at(text_comp.font_id).font);
            text.setCharacterSize(text_comp.font_size);
            text.setLineSpacing(text_comp.line_spacing_factor);
            text.setLetterSpacing(appear.letter_spacing_factor);
            text.setStyle(cast::ToSfTextStyle(appear.style));
            text.setFillColor(cast::ToSfColor(appear.fill_color));
            text.setOutlineColor(cast::ToSfColor(appear.outline_color));
            text.setOutlineThickness(appear.outline_thickness);
        };

        for (auto itr = text_comp.character_appearances.begin(); itr != text_comp.character_appearances.end();)
        {
            auto& [start, appear] = *itr;
            auto end = text_comp.string.size();
            if (++itr != text_comp.character_appearances.end())
                end = itr->first;
            
            auto& text = texts[start];

            UpdateText(text, start, end, appear);
        }
    }

    Rectf Text::GetAABB() const
    {
        Rectf aabb;

        for (auto& [index, text] : texts)
        {
            Rectf local_aabb = cast::FromSfRect(text.getGlobalBounds());

            aabb.top = std::min(local_aabb.top, aabb.top);
            aabb.left = std::min(local_aabb.left, aabb.left);
            aabb.height = std::max(local_aabb.top + local_aabb.height, aabb.height);
            aabb.width = std::max(local_aabb.left + local_aabb.width, aabb.width);
        }

        aabb.height -= aabb.top;
        aabb.width -= aabb.left;

        return aabb;
    }

    void Text::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        for (auto& [index, text] : texts)
        {
            target.draw(text, states);
        }
    }
}