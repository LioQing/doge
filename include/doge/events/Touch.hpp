#pragma once

#include <doge/utils/lev.hpp>
#include <doge/utils/Vec2.hpp>

namespace doge::event
{
    struct Touch
    {
        std::uint32_t finger;
        Vec2i position;
    };
};