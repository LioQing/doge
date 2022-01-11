#pragma once

#include <doge/utils/lev.hpp>

namespace doge::event
{
    struct ControllerMove
    {
        std::uint8_t id;
        std::uint8_t axis;
        float position;
    };
};