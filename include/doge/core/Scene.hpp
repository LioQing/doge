#pragma once

#include <functional>

#include "aliases.hpp"

namespace doge
{
    struct Scene
    {
        std::function<void(Engine&)> start;
        std::function<void(Engine&, DeltaTime)> update;
        std::function<void(Engine&, DeltaTime)> fixed_update;
        std::function<void(Engine&)> finish;
    };
}