#pragma once

#include <doge/utils/lev.hpp>
#include <doge/utils/Vec3.hpp>
#include <doge/core/io/InputDevice.hpp>

namespace doge::event
{
    struct Sensor
    {
        io::InputDevice::Sensor::Type type;
        Vec3f value;
    };
};