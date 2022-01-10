#include <doge/core/io/InputDevice.hpp>

namespace doge::io
{
    bool InputDevice::Keyboard::IsKeyDown(Key key)
    {
        return sf::Keyboard::isKeyPressed(key);
    }

    void InputDevice::Keyboard::SetVirtualKeyboardVisible(bool visible)
    {
        sf::Keyboard::setVirtualKeyboardVisible(visible);
    }
}