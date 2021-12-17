#include <doge/utils/cast.hpp>

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

    float cast::ToDegree(float radian)
    {
        return radian * 180.f / 3.1415926535f;
    }

    float cast::ToRadian(float degree)
    {
        return degree * 3.1415926535f / 180.f;
    }
}