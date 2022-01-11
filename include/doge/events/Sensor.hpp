#pragma once

#include <doge/utils/lev.hpp>
#include <doge/utils/Vec3.hpp>
#include <doge/core/io/Input.hpp>

namespace doge::event
{
    struct Sensor
    {
        io::Input::Sensor::Type type;
        Vec3f value;
    };
};