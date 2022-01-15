#pragma once

#include <doge/utils/aliases.hpp>
#include <doge/extensions/fsm/StateMachineAutoUpdate.hpp>

namespace doge
{
    struct Engine;
    struct Entity;
    struct StateMachine;

    struct fsm
    {
        using State = std::uint32_t;

        fsm(const fsm&) = delete;
        static void Enable(Engine& engine);
        static void Disable(Engine& engine);

        static void ManualTransition(StateMachine& state_machine, fsm::State new_state, Entity entity, Engine& engine, DeltaTime dt = 0, bool force_call_event = false);
        static void ManualUpdate(Engine& engine, DeltaTime dt = 0, StateMachineAutoUpdate auto_update = StateMachineAutoUpdate::All);

    private:

        static void Start(Engine& engine);
        static void Update(Engine& engine, DeltaTime dt);
        static void FixedUpdate(Engine& engine, DeltaTime dt);
        static void Finish(Engine& engine);
    };
}