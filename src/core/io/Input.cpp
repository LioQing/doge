#include <doge/core/io/Input.hpp>

#include <doge/utils.hpp>
#include <doge/core/io.hpp>

namespace doge::io
{
    bool Input::Keyboard::IsKeyDown(Key key)
    {
        return sf::Keyboard::isKeyPressed(key);
    }

    void Input::Keyboard::SetVirtualKeyboardVisible(bool visible)
    {
        sf::Keyboard::setVirtualKeyboardVisible(visible);
    }

    void Input::Mouse::SetPosition(const Vec2i& position)
    {
        sf::Mouse::setPosition(cast::ToSfVec2(position));
    }

    void Input::Mouse::SetPosition(const Vec2i& position, const Window& window)
    {
        sf::Mouse::setPosition(cast::ToSfVec2(position), window.window);
    }

    Vec2i Input::Mouse::GetPosition()
    {
        return cast::FromSfVec2(sf::Mouse::getPosition());
    }

    Vec2i Input::Mouse::GetPosition(const Window& window)
    {
        return cast::FromSfVec2(sf::Mouse::getPosition(window.window));
    }

    bool Input::Controller::IsConnected(std::uint8_t id)
    {
        sf::Joystick::update();
        return sf::Joystick::isConnected(id);
    }

    std::uint8_t Input::Controller::GetButtonCount(std::uint8_t id)
    {
        sf::Joystick::update();
        return sf::Joystick::getButtonCount(id);
    }

    bool Input::Controller::HasAxis(std::uint8_t id, std::uint8_t axis)
    {
        sf::Joystick::update();
        return sf::Joystick::hasAxis(id, cast::ToSfControllerAxis(axis));
    }

    bool Input::Controller::IsButtonDown(std::uint8_t id, std::uint8_t button)
    {
        sf::Joystick::update();
        return sf::Joystick::isButtonPressed(id, button);
    }

    float Input::Controller::GetAxisPosition(std::uint8_t id, std::uint8_t axis)
    {
        sf::Joystick::update();
        return sf::Joystick::getAxisPosition(id, cast::ToSfControllerAxis(axis));
    }

    Input::Controller::Info Input::Controller::GetInfo(std::uint8_t id)
    {
        sf::Joystick::update();
        sf::Joystick::Identification info = sf::Joystick::getIdentification(id);
        return Info(info.name, info.vendorId, info.productId);
    }

    bool Input::Touch::IsDown(std::uint32_t finger)
    {
        return sf::Touch::isDown(finger);
    }

    Vec2i Input::Touch::GetPosition(std::uint32_t finger)
    {
        return cast::FromSfVec2(sf::Touch::getPosition(finger));
    }

    Vec2i Input::Touch::GetPosition(std::uint32_t finger, const Window& window)
    {
        return cast::FromSfVec2(sf::Touch::getPosition(finger, window.window));
    }

    bool Input::Sensor::IsAvailable(Type sensor)
    {
        return sf::Sensor::isAvailable(cast::ToSfSensorType(sensor));
    }

    void Input::Sensor::SetEnabled(Type sensor, bool enabled)
    {
        sf::Sensor::setEnabled(cast::ToSfSensorType(sensor), enabled);
    }

    Vec3f Input::Sensor::GetValue(Type sensor)
    {
        return cast::FromSfVec3(sf::Sensor::getValue(cast::ToSfSensorType(sensor)));
    }
}