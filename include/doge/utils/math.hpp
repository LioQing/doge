#pragma once

namespace doge
{
    struct math
    {
        math(const math&) = delete;

        static float ConstrainAngle(float rad);
    };
}