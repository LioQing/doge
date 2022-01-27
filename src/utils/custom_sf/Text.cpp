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
            std::size_t i;
            for (i = 0; i < index; ++i)
            {
                if (text_comp.string.at(i) == '\n')
                {
                    ++line;
                    offset.x = 0.f;
                }
                else
                {
                    offset.x += font.GetLetterSpacing(
                        text_comp.string.at(i),
                        text_comp.string.at(i + 1),
                        text_comp.font_size,
                        appear.style & doge::Text::Style::Bold,
                        appear.outline_thickness)
                        * appear.letter_spacing_factor;
                }
            }
            
            offset.y = line * font.GetLineSpacing(text_comp.font_size) * text_comp.line_spacing_factor;

            if (text_comp.align == doge::Text::Align::Left)
                return offset;
            
            float line_width = offset.x;
            for (; i + 1 < text_comp.string.size() && text_comp.string.at(i) != '\n'; ++i)
            {
                line_width += font.GetLetterSpacing(
                    text_comp.string.at(i),
                    text_comp.string.at(i + 1),
                    text_comp.font_size,
                    appear.style & doge::Text::Style::Bold,
                    appear.outline_thickness)
                    * appear.letter_spacing_factor;
            }
            
            if (i + 1 == text_comp.string.size())
            {
                line_width += font.GetGlyph(
                    text_comp.string.at(i),
                    text_comp.font_size,
                    appear.style & doge::Text::Style::Bold,
                    appear.outline_thickness).advance;
            }

            if (text_comp.align == doge::Text::Align::Right)
                return offset - Vec2f(line_width, 0);
            
            return offset - Vec2f(line_width / 2.f, 0);
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

        std::map<std::size_t, const doge::Text::Appearance&> sep_texts;
        for (auto& [i, appear] : text_comp.character_appearances)
        {
            sep_texts.emplace(i, appear);
        }

        auto prev_itr = sep_texts.begin();
        for (std::size_t i = 1; i < text_comp.string.size(); ++i)
        {
            auto itr = sep_texts.find(i);
            if (itr != sep_texts.end())
            {
                auto& [start, appear] = *prev_itr;
                auto& end = itr->first;
                
                auto& text = texts[start];

                UpdateText(text, start, end, appear);

                prev_itr = itr;
            }

            if (text_comp.string.at(i) == '\n')
            {
                sep_texts.emplace(i + 1, prev_itr->second);
            }
        }
        UpdateText(texts[prev_itr->first], prev_itr->first, text_comp.string.size(), prev_itr->second);
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