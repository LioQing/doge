#pragma once

#include <box2d/box2d.h>
#include <doge/utils/Vec2.hpp>

namespace doge
{
    struct PhysicsCollider
    {
        enum Type
        {
            Convex, Circle, Rectangle, Edge,
            Count
        };

        b2Fixture* b2_fixture;

        void SetSensor(bool sensor);
        bool IsSensor() const;

        bool TestPoint(const Vec2f& point);
    };
}