#pragma once

#include <doge/utils/lev.hpp>
#include <doge/utils/Vec3.hpp>
#include <doge/core/io/Sensor.hpp>

namespace doge::event
{
    struct Sensor
    {
        using Type = io::Sensor::Type;

        Type type;
        Vec3f value;
    };
};