#include <doge/components/Scale.hpp>

namespace doge
{
    Scale::Scale(const Vec2f& scale) : scale(scale) 
    {
    }

    Scale::Scale(float x, float y) : scale(x, y) 
    {
    }
}