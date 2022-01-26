#include <doge/utils/custom_sf/DrawableVertices.hpp>

namespace doge::custom_sf
{
    void DrawableVertices::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.texture = texture;
        target.draw(vertices.data(), vertices.size(), type, states);
    }
}