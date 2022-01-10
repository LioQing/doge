#include <doge/components/Velocity.hpp>

namespace doge
{
    Velocity::Velocity(const Vec2f& velocity) : velocity(velocity) 
    {
    }

    Velocity::Velocity(float x, float y) : velocity(x, y) 
    {
    }
}