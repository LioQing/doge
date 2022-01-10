#include <doge/core/io/InputDevice.hpp>

#include <doge/utils.hpp>
#include <doge/core/io.hpp>

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

    void InputDevice::Mouse::SetPosition(const Vec2i& position)
    {
        sf::Mouse::setPosition(cast::ToSfVec2(position));
    }

    void InputDevice::Mouse::SetPosition(const Vec2i& position, const Window& window)
    {
        sf::Mouse::setPosition(cast::ToSfVec2(position), *window.window_sptr);
    }

    Vec2i InputDevice::Mouse::GetPosition()
    {
        return cast::FromSfVec2(sf::Mouse::getPosition());
    }

    Vec2i InputDevice::Mouse::GetPosition(const Window& window)
    {
        return cast::FromSfVec2(sf::Mouse::getPosition(*window.window_sptr));
    }

    void InputDevice::SetRelativeMousePosition(const Vec2i& position) const
    {
        if (std::shared_ptr<sf::RenderWindow> sptr = window_wptr.lock())
            sf::Mouse::setPosition(cast::ToSfVec2(position), *sptr);
        else
            throw std::invalid_argument("No sf::RenderWindow is referenced in doge::io::InputDevice");
    }

    Vec2i InputDevice::GetRelativeMousePosition() const
    {
        if (std::shared_ptr<sf::RenderWindow> sptr = window_wptr.lock())
            return cast::FromSfVec2(sf::Mouse::getPosition(*sptr));
        else
            throw std::invalid_argument("No sf::RenderWindow is referenced in doge::io::InputDevice");
    }
}