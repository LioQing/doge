#include <doge/utils/cast.hpp>

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <doge/components.hpp>
#include <doge/utils.hpp>
#include <doge/core.hpp>

namespace doge
{
    Color cast::FromSfColor(const sf::Color& color)
    {
        return Color(color.r, color.g, color.b, color.a);
    }

    sf::Color cast::ToSfColor(const Color& color)
    {
        return sf::Color(color.r, color.g, color.b, color.a);
    }

    float cast::ToDegree(float radian)
    {
        return radian * 180.f / 3.1415926535f;
    }

    float cast::ToRadian(float degree)
    {
        return degree * 3.1415926535f / 180.f;
    }

    WindowSettings::Style cast::FromSfStyle(int style)
    {
        switch (style)
        {
            case sf::Style::None:           return WindowSettings::Style::None;
            case sf::Style::Titlebar:       return WindowSettings::Style::Titlebar;
            case sf::Style::Resize:         return WindowSettings::Style::Resizable;
            case sf::Style::Close:          return WindowSettings::Style::Close;
            case sf::Style::Fullscreen:     return WindowSettings::Style::Fullscreen;
            default: return WindowSettings::Style::Default;
        }
    }

    int cast::ToSfStyle(WindowSettings::Style style)
    {
        switch (style)
        {
            case WindowSettings::Style::None:       return sf::Style::None;
            case WindowSettings::Style::Titlebar:   return sf::Style::Titlebar;
            case WindowSettings::Style::Resizable:  return sf::Style::Resize;
            case WindowSettings::Style::Close:      return sf::Style::Close;
            case WindowSettings::Style::Fullscreen: return sf::Style::Fullscreen;
            default: return sf::Style::Default;
        }
    }

    io::Keyboard::Key cast::FromSfKeyboardKey(sf::Keyboard::Key key)
    {
        return key;
    }

    sf::Keyboard::Key cast::ToSfKeyboardKey(io::Keyboard::Key key)
    {
        return key;        
    }

    io::Mouse::Wheel cast::FromSfMouseWheel(sf::Mouse::Wheel wheel)
    {
        using dwheel = io::Mouse::Wheel;
        using sfwheel = sf::Mouse::Wheel;

        return wheel == sfwheel::HorizontalWheel ? dwheel::Horizontal : dwheel::Vertical;
    }

    sf::Mouse::Wheel cast::ToSfMouseWheel(io::Mouse::Wheel wheel)
    {
        using dwheel = io::Mouse::Wheel;
        using sfwheel = sf::Mouse::Wheel;

        return wheel == dwheel::Horizontal ? sfwheel::HorizontalWheel : sfwheel::VerticalWheel;
    }

    io::Mouse::Button cast::FromSfMouseButton(sf::Mouse::Button button)
    {
        using dbutton = io::Mouse::Button;
        using sfbutton = sf::Mouse::Button;

        switch (button)
        {
            case sfbutton::Left:        return dbutton::Left;
            case sfbutton::Right:       return dbutton::Right;
            case sfbutton::Middle:      return dbutton::Middle;
            case sfbutton::XButton1:    return dbutton::Mouse4;
            case sfbutton::XButton2:    return dbutton::Mouse5;
            default: return dbutton::Count;
        }
    }

    sf::Mouse::Button cast::ToSfMouseButton(io::Mouse::Button button)
    {
        using dbutton = io::Mouse::Button;
        using sfbutton = sf::Mouse::Button;

        switch (button)
        {
            case dbutton::Left:     return sfbutton::Left;
            case dbutton::Right:    return sfbutton::Right;
            case dbutton::Middle:   return sfbutton::Middle;
            case dbutton::Mouse4:   return sfbutton::XButton1;
            case dbutton::Mouse5:   return sfbutton::XButton2;
            default: return sfbutton::ButtonCount;
        }
    }

    io::Sensor::Type cast::FromSfSensorType(sf::Sensor::Type type)
    {
        using dtype = io::Sensor::Type;
        using sftype = sf::Sensor::Type;

        switch (type)
        {
            case sftype::Accelerometer:         return dtype::Accelerometer;
            case sftype::Gyroscope:             return dtype::Gyroscope;
            case sftype::Magnetometer:          return dtype::Magnetometer;
            case sftype::Gravity:               return dtype::Gravity;
            case sftype::UserAcceleration:      return dtype::UserAcceleration;
            case sftype::Orientation:           return dtype::Orientation;
            default: return dtype::Count;
        }
    }

    sf::Sensor::Type cast::ToSfSensorType(io::Sensor::Type type)
    {
        using dtype = io::Sensor::Type;
        using sftype = sf::Sensor::Type;

        switch (type)
        {
            case dtype::Accelerometer:         return sftype::Accelerometer;
            case dtype::Gyroscope:             return sftype::Gyroscope;
            case dtype::Magnetometer:          return sftype::Magnetometer;
            case dtype::Gravity:               return sftype::Gravity;
            case dtype::UserAcceleration:      return sftype::UserAcceleration;
            case dtype::Orientation:           return sftype::Orientation;
            default: return sftype::Count;
        }
    }

    io::Event::Content cast::FromSfEvent(const sf::Event& event)
    {
        using io::Event;

        if (event.type == sf::Event::EventType::Closed)
            return Event::Content(Event::Type::Closed);
        if (event.type == sf::Event::EventType::Resized)
            return Event::Content(Event::Type::Resized, event::Size(Vec2u(event.size.width, event.size.height)));
        if (event.type == sf::Event::EventType::LostFocus)
            return Event::Content(Event::Type::LostFocus);
        if (event.type == sf::Event::EventType::GainedFocus)
            return Event::Content(Event::Type::GainedFocus);
        if (event.type == sf::Event::EventType::TextEntered)
            return Event::Content(Event::Type::TextEntered, event::Text(event.text.unicode));
        if (event.type == sf::Event::EventType::KeyPressed)
            return Event::Content(Event::Type::KeyPressed, event::Key(cast::FromSfKeyboardKey(event.key.code), event.key.alt, event.key.control, event.key.shift, event.key.system));
        if (event.type == sf::Event::EventType::KeyReleased)
            return Event::Content(Event::Type::KeyReleased, event::Key(cast::FromSfKeyboardKey(event.key.code), event.key.alt, event.key.control, event.key.shift, event.key.system));
        if (event.type == sf::Event::EventType::MouseWheelScrolled)
            return Event::Content(Event::Type::MouseWheelScrolled, event::MouseWheel(cast::FromSfMouseWheel(event.mouseWheelScroll.wheel), event.mouseWheelScroll.delta, Vec2i(event.mouseWheelScroll.x, event.mouseWheelScroll.y)));
        if (event.type == sf::Event::EventType::MouseButtonPressed)
            return Event::Content(Event::Type::MouseButtonPressed, event::MouseButton(cast::FromSfMouseButton(event.mouseButton.button), Vec2i(event.mouseButton.x, event.mouseButton.y)));
        if (event.type == sf::Event::EventType::MouseButtonReleased)
            return Event::Content(Event::Type::MouseButtonReleased, event::MouseButton(cast::FromSfMouseButton(event.mouseButton.button), Vec2i(event.mouseButton.x, event.mouseButton.y)));
        if (event.type == sf::Event::EventType::MouseMoved)
            return Event::Content(Event::Type::MouseMoved, event::MouseMove(Vec2i(event.mouseMove.x, event.mouseMove.y)));
        if (event.type == sf::Event::EventType::MouseEntered)
            return Event::Content(Event::Type::MouseEntered);
        if (event.type == sf::Event::EventType::MouseLeft)
            return Event::Content(Event::Type::MouseLeft);
        if (event.type == sf::Event::EventType::JoystickButtonPressed)
            return Event::Content(Event::Type::ControllerButtonPressed, event::ControllerButton(event.joystickButton.joystickId, event.joystickButton.button));
        if (event.type == sf::Event::EventType::JoystickButtonReleased)
            return Event::Content(Event::Type::ControllerButtonReleased, event::ControllerButton(event.joystickButton.joystickId, event.joystickButton.button));
        if (event.type == sf::Event::EventType::JoystickMoved)
            return Event::Content(Event::Type::ControllerMoved, event::ControllerMove(event.joystickMove.joystickId, event.joystickMove.axis, event.joystickMove.position));
        if (event.type == sf::Event::EventType::JoystickConnected)
            return Event::Content(Event::Type::ControllerConnected, event::ControllerConnect(event.joystickConnect.joystickId));
        if (event.type == sf::Event::EventType::JoystickDisconnected)
            return Event::Content(Event::Type::ControllerDisconnected, event::ControllerConnect(event.joystickConnect.joystickId));
        if (event.type == sf::Event::EventType::TouchBegan)
            return Event::Content(Event::Type::TouchBegan, event::Touch(event.touch.finger, Vec2i(event.touch.x, event.touch.y)));
        if (event.type == sf::Event::EventType::TouchMoved)
            return Event::Content(Event::Type::TouchMoved, event::Touch(event.touch.finger, Vec2i(event.touch.x, event.touch.y)));
        if (event.type == sf::Event::EventType::TouchEnded)
            return Event::Content(Event::Type::TouchEnded, event::Touch(event.touch.finger, Vec2i(event.touch.x, event.touch.y)));
        if (event.type == sf::Event::EventType::SensorChanged)
            return Event::Content(Event::Type::SensorChanged, event::Sensor(cast::FromSfSensorType(event.sensor.type), Vec3f(event.sensor.x, event.sensor.y, event.sensor.z)));
        return Event::Content(Event::Type::Count);
    }

    CustomShape::Type cast::FromSfPolygonType(sf::PrimitiveType type)
    {
        switch (type)
        {
            case sf::PrimitiveType::Points:             return CustomShape::Type::Points;
            case sf::PrimitiveType::Lines:              return CustomShape::Type::Lines;
            case sf::PrimitiveType::LineStrip:          return CustomShape::Type::LineStrip;
            case sf::PrimitiveType::Triangles:          return CustomShape::Type::Triangles;
            case sf::PrimitiveType::TriangleStrip:      return CustomShape::Type::TriangleStrip;
            case sf::PrimitiveType::TriangleFan:        return CustomShape::Type::TriangleFan;
            case sf::PrimitiveType::Quads:              return CustomShape::Type::Quads;
            default: throw std::invalid_argument("Invalid CustomShape::Type");
        }
    }

    sf::PrimitiveType cast::ToSfPolygonType(CustomShape::Type type)
    {
        switch (type)
        {
            case CustomShape::Type::Points:            return sf::PrimitiveType::Points;
            case CustomShape::Type::Lines:             return sf::PrimitiveType::Lines;
            case CustomShape::Type::LineStrip:         return sf::PrimitiveType::LineStrip;
            case CustomShape::Type::Triangles:         return sf::PrimitiveType::Triangles;
            case CustomShape::Type::TriangleStrip:     return sf::PrimitiveType::TriangleStrip;
            case CustomShape::Type::TriangleFan:       return sf::PrimitiveType::TriangleFan;
            case CustomShape::Type::Quads:             return sf::PrimitiveType::Quads;
            default: throw std::invalid_argument("Invalid CustomShape::Type");
        }
    }

    std::uint8_t cast::FromSfControllerAxis(sf::Joystick::Axis axis)
    {
        using a = sf::Joystick::Axis;

        switch (axis)
        {
            case a::X:      return 0;
            case a::Y:      return 1;
            case a::Z:      return 2;
            case a::R:      return 3;
            case a::U:      return 4;
            case a::V:      return 5;
            case a::PovX:   return 6;
            case a::PovY:   return 7;
            default:        return -1;
        }
    }

    sf::Joystick::Axis cast::ToSfControllerAxis(std::uint8_t axis)
    {
        using a = sf::Joystick::Axis;

        switch (axis)
        {
            case 0: return a::X;
            case 1: return a::Y;
            case 2: return a::Z;
            case 3: return a::R;
            case 4: return a::U;
            case 5: return a::V;
            case 6: return a::PovX;
            case 7: return a::PovY;
            default: throw std::invalid_argument("Invalid controller axis");
        }
    }

    Text::Style cast::FromSfTextStyle(std::uint32_t style)
    {
        Text::Style dstyle = Text::Style::Regular;

        if (style & sf::Text::Bold)             style |= Text::Style::Bold;
        if (style & sf::Text::Italic)           style |= Text::Style::Italic;
        if (style & sf::Text::Underlined)       style |= Text::Style::Underlined;
        if (style & sf::Text::StrikeThrough)    style |= Text::Style::StrikeThrough;

        return dstyle;
    }

    std::uint32_t cast::ToSfTextStyle(Text::Style style)
    {
        std::uint32_t sftext = sf::Text::Regular;

        if (style & Text::Style::Bold)             sftext |= sf::Text::Bold;
        if (style & Text::Style::Italic)           sftext |= sf::Text::Italic;
        if (style & Text::Style::Underlined)       sftext |= sf::Text::Underlined;
        if (style & Text::Style::StrikeThrough)    sftext |= sf::Text::StrikeThrough;

        return sftext;
    }
}