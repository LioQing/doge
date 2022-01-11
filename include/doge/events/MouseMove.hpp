#pragma once

#include <doge/utils/lev.hpp>
#include <doge/utils/Vec2.hpp>

namespace doge::event
{
    struct MouseMove
    {
        Vec2i position;
    };
};