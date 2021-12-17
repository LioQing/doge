#pragma once

#include <concepts>
#include <doge/components.hpp>
#include <doge/utils/Vec2.hpp>
#include <doge/utils/lic.hpp>
#include <doge/utils/aliases.hpp>

namespace doge
{
    struct Entity;

    struct global
    {
        global(const global&) = delete;

        static Vec2f GetPosition(Entity& entity);

        static Vec2f GetScale(Entity& entity);

        static float GetRotation(Entity& entity);
    };
}