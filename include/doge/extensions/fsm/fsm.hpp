#pragma once

#include <doge/utils/aliases.hpp>

namespace doge
{
    struct Engine;

    struct fsm
    {
        using State = std::uint64_t;

        fsm(const fsm&) = delete;
        static void Enable(Engine& engine);
        static void Disable(Engine& engine);

        static void ManualUpdate(Engine& engine, DeltaTime dt = 0, bool is_fixed_update = false);

    private:

        static void Start(Engine& engine);
        static void Update(Engine& engine, DeltaTime dt);
        static void FixedUpdate(Engine& engine, DeltaTime dt);
        static void Finish(Engine& engine);
    };
}