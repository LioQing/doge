#pragma once

#include <SFML/Graphics.hpp>
#include <doge/utils/Rect.hpp>
#include <unordered_map>

namespace doge
{
    struct Text;
    struct Assets;

    namespace custom_sf
    {
        struct Text : sf::Drawable, sf::Transformable
        {
            Text(const Assets& assets, const doge::Text& text_component);

            void Update(const Assets& assets, const doge::Text& text_component);

            Rectf GetAABB() const;

        private:

            void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

            std::unordered_map<std::size_t, sf::Text> texts;
        };
    }
}