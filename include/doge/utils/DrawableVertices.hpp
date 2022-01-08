#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

namespace doge
{
    struct DrawableVertices : sf::Drawable
    {
        std::vector<sf::Vertex> vertices;
        sf::PrimitiveType type;
        const sf::Texture* texture;

    private:

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
        {
            states.texture = texture;
            target.draw(vertices.data(), vertices.size(), type, states);
        }
    };
}