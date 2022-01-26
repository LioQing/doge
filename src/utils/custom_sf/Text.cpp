#include <doge/utils/custom_sf/Text.hpp>

#include <doge/components/Text.hpp>
#include <doge/utils/cast.hpp>
#include <doge/core/io/Font.hpp>
#include <doge/core/Assets.hpp>

namespace doge::custom_sf
{
    Text::Text(const Assets& assets, const doge::Text& text_component)
    {
        Update(assets, text_component);
    }

    void Text::Update(const Assets& assets, const doge::Text& text_component)
    {
        text.setPosition(getPosition());
        text.setScale(getScale());
        text.setRotation(getRotation());
        text.setOrigin(getOrigin());
        
        sf::String string;
        for (auto c : text_component.string)
        {
            string += sf::Uint32(c);
        }
        text.setString(string);
        text.setFont(assets.fonts.at(text_component.font_id).font);
        text.setCharacterSize(text_component.font_size);
        text.setLineSpacing(text_component.line_spacing_factor);
        text.setLetterSpacing(text_component.character_appearances.at(0).letter_spacing_factor);
        text.setStyle(cast::ToSfTextStyle(text_component.character_appearances.at(0).style));
        text.setFillColor(cast::ToSfColor(text_component.character_appearances.at(0).fill_color));
        text.setOutlineColor(cast::ToSfColor(text_component.character_appearances.at(0).outline_color));
        text.setOutlineThickness(text_component.character_appearances.at(0).outline_thickness);
    }

    Rectf Text::GetAABB() const
    {
        return cast::FromSfRect(text.getGlobalBounds());
    }

    void Text::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        target.draw(text, states);
    }
}