#include <doge/core/io/Touch.hpp>

#include <doge/utils.hpp>
#include <doge/core/io.hpp>

namespace doge::io
{
    bool Touch::IsDown(std::uint32_t finger)
    {
        return sf::Touch::isDown(finger);
    }

    Vec2i Touch::GetPosition(std::uint32_t finger)
    {
        return cast::FromSfVec2(sf::Touch::getPosition(finger));
    }

    Vec2i Touch::GetPosition(std::uint32_t finger, const Window& window)
    {
        return cast::FromSfVec2(sf::Touch::getPosition(finger, window.window));
    }
}