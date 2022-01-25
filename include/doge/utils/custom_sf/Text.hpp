#pragma once

#include <SFML/Graphics.hpp>

namespace doge
{
    struct Text;

    namespace custom_sf
    {
        struct Text : sf::Drawable, sf::Transformable
        {
            Text& text_component;

            void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        };
    }
}