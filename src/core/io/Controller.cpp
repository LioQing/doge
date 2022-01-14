#include <doge/core/io/Controller.hpp>

#include <doge/utils.hpp>
#include <doge/core/io.hpp>

namespace doge::io
{
    bool Controller::IsConnected(std::uint8_t id)
    {
        sf::Joystick::update();
        return sf::Joystick::isConnected(id);
    }

    std::uint8_t Controller::GetButtonCount(std::uint8_t id)
    {
        sf::Joystick::update();
        return sf::Joystick::getButtonCount(id);
    }

    bool Controller::HasAxis(std::uint8_t id, std::uint8_t axis)
    {
        sf::Joystick::update();
        return sf::Joystick::hasAxis(id, cast::ToSfControllerAxis(axis));
    }

    bool Controller::IsButtonDown(std::uint8_t id, std::uint8_t button)
    {
        sf::Joystick::update();
        return sf::Joystick::isButtonPressed(id, button);
    }

    float Controller::GetAxisPosition(std::uint8_t id, std::uint8_t axis)
    {
        sf::Joystick::update();
        return sf::Joystick::getAxisPosition(id, cast::ToSfControllerAxis(axis));
    }

    Controller::Info Controller::GetInfo(std::uint8_t id)
    {
        sf::Joystick::update();
        sf::Joystick::Identification info = sf::Joystick::getIdentification(id);
        return Info(info.name, info.vendorId, info.productId);
    }
}