#include <doge/utils/conversions.hpp>

namespace doge
{
    Color conversions::FromSfColor(const sf::Color& color)
    {
        return Color(color.r, color.g, color.b, color.a);
    }

    sf::Color conversions::ToSfColor(const Color& color)
    {
        return sf::Color(color.r, color.g, color.b, color.a);
    }

    float conversions::ToDegree(float radian)
    {
        return radian * 180.f / 3.1415926535f;
    }

    float conversions::ToRadian(float degree)
    {
        return degree * 3.1415926535f / 180.f;
    }
}