#pragma once

#include <functional>
#include <concepts>
#include <doge/utils/aliases.hpp>

namespace doge
{
    struct Engine;

    struct GameLoopFunctions
    {
        std::function<void(Engine&)> start;
        std::function<void(Engine&, DeltaTime)> fixed_update;
        std::function<void(Engine&, DeltaTime)> early_update;
        std::function<void(Engine&, DeltaTime)> update;
        std::function<void(Engine&, DeltaTime)> late_update;
        std::function<void(Engine&)> render;
        std::function<void(Engine&)> finish;
    };
}