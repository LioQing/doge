#pragma once

#include <SFML/Graphics.hpp>
#include <doge/utils/Vec2.hpp>
#include <doge/utils/Vec3.hpp>

namespace doge::io
{
    struct Sensor
    {
        enum Type
        {
            Accelerometer,
            Gyroscope,
            Magnetometer,
            Gravity,
            UserAcceleration,
            Orientation,
            
            Count
        };

        Sensor(const Sensor&) = delete;

        static bool IsAvailable(Type sensor);
        static void SetEnabled(Type sensor, bool enabled);
        static Vec3f GetValue(Type sensor);
    };
}