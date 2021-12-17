#pragma once

namespace doge
{
    struct math
    {
        math(const math&) = delete;

        static constexpr float pi = 3.1415926535;

        static float ConstrainAngle(float rad);
    };
}