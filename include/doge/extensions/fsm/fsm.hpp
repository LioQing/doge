#pragma once

#include <doge/utils/aliases.hpp>

namespace doge
{
    struct Engine;

    struct fsm
    {
        using StateType = std::uint32_t;

        fsm(const fsm&) = delete;
        static void Enable(Engine& engine);
        static void Disable(Engine& engine);

    private:

        static void Start(Engine& engine);
        static void Update(Engine& engine, DeltaTime dt);
        static void FixedUpdate(Engine& engine, DeltaTime dt);
        static void Finish(Engine& engine);
    };
}