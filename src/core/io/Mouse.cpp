#include <doge/core/io/Mouse.hpp>

#include <doge/utils.hpp>

namespace doge::io
{
    void Mouse::SetPosition(const Vec2i& position)
    {
        sf::Mouse::setPosition(cast::ToSfVec2(position));
    }

    void Mouse::SetPosition(const Vec2i& position, const Window& window)
    {
        sf::Mouse::setPosition(cast::ToSfVec2(position), window.window);
    }

    Vec2i Mouse::GetPosition()
    {
        return cast::FromSfVec2(sf::Mouse::getPosition());
    }

    Vec2i Mouse::GetPosition(const Window& window)
    {
        return cast::FromSfVec2(sf::Mouse::getPosition(window.window));
    }
}