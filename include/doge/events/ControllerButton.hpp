#pragma once

#include <doge/utils/lev.hpp>

namespace doge::event
{
    struct ControllerButton
    {
        std::uint8_t id;
        std::uint8_t button;
    };
};