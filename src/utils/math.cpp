#include <doge/utils/math.hpp>

#include <cmath>

namespace doge
{
    float math::ConstrainAngle(float rad)
    {
        rad = std::fmod(rad + pi, pi * 2.f);
        if (rad < 0)
            rad += pi * 2.f;
        return rad - pi;
    }
}