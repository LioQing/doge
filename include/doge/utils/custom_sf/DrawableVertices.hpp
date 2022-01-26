#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

namespace doge::custom_sf
{
    struct DrawableVertices : sf::Drawable
    {
        std::vector<sf::Vertex> vertices;
        sf::PrimitiveType type;
        const sf::Texture* texture;

    private:

        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    };
}