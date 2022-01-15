#pragma once

#include <doge/extensions/fsm/fsm.hpp>
#include <doge/utils.hpp>
#include <functional>

namespace doge
{
    struct Engine;

    struct StateMachine
    {
        fsm::State state;
        std::function<fsm::State(fsm::State, Entity, Engine&, DeltaTime)> transition;
        bool is_fixed_update = false;

        Event<fsm::State, Entity, Engine&, DeltaTime> on_entry;
        Event<fsm::State, Entity, Engine&, DeltaTime> on_exit;
    };
}