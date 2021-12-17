#pragma once

#include <concepts>
#include <doge/components.hpp>
#include <doge/utils/Vec2.hpp>
#include <doge/utils/lic.hpp>
#include <doge/utils/aliases.hpp>

namespace doge
{
    struct global
    {
        global(const global&) = delete;

        static Vec2f GetPosition(EntityID eid);

        static Vec2f GetScale(EntityID eid);

        static float GetRotation(EntityID eid);
    };
}