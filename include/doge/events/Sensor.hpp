#pragma once

#include <doge/utils/lev.hpp>
#include <doge/utils/Vec3.hpp>
#include <doge/core/io/Input.hpp>

namespace doge::event
{
    struct Sensor
    {
        using Type = io::Input::Sensor::Type;

        Type type;
        Vec3f value;
    };
};