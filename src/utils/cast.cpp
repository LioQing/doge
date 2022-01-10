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
    
    Vec2f cast::FromB2Vec2(const b2Vec2& v)
    {
        return Vec2f(v.x / ToB2Scale, v.y / ToB2Scale);
    }

    b2Vec2 cast::ToB2Vec2(const Vec2f& v)
    {
        return b2Vec2(v.x * ToB2Scale, v.y * ToB2Scale);
    }

    float cast::FromB2Length(float l)
    {
        return l / ToB2Scale;
    }

    float cast::ToB2Length(float l)
    {
        return l * ToB2Scale;
    }

    b2BodyType cast::ToB2BodyType(RigidBody::Type type)
    {
        switch (type)
        {
            case RigidBody::Type::Static: return b2BodyType::b2_staticBody;
            case RigidBody::Type::Kinematic: return b2BodyType::b2_kinematicBody;
            case RigidBody::Type::Dynamic: return b2BodyType::b2_dynamicBody;
            default: throw std::invalid_argument("False b2 body type");
        }
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
}