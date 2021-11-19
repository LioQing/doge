#pragma once

#include <functional>

namespace doge
{
    struct Engine;

    using DeltaTime = float;

    struct Scene
    {
        std::function<void(Engine&)> start;
        std::function<void(Engine&, DeltaTime)> update;
        std::function<void(Engine&, DeltaTime)> fixed_update;
    };
}