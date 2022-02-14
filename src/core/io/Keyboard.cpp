#include <doge/core/io/Keyboard.hpp>

#include <doge/utils.hpp>

namespace doge::io
{
    bool Keyboard::IsKeyDown(Key key)
    {
        return sf::Keyboard::isKeyPressed(key);
    }

    void Keyboard::SetVirtualKeyboardVisible(bool visible)
    {
        sf::Keyboard::setVirtualKeyboardVisible(visible);
    }
}