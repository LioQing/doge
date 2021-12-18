#include <doge/utils/cast.hpp>

#include <doge/components.hpp>

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
        return Vec2f(v.x, v.y);
    }

    b2Vec2 cast::ToB2Vec2(const Vec2f& v)
    {
        return b2Vec2(v.x, v.y);
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
}