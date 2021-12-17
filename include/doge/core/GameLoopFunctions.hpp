#pragma once

#include <functional>
#include <doge/utils.hpp>

namespace doge
{
    struct GameLoopFunctions
    {
        std::function<void(Engine&)> start;
        std::function<void(Engine&, DeltaTime)> update;
        std::function<void(Engine&, DeltaTime)> fixed_update;
        std::function<void(Engine&)> finish;
    };
}