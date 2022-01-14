#pragma once

#include <doge/extensions/fsm/fsm.hpp>
#include <doge/utils.hpp>
#include <functional>
#include <concepts>
#include <type_traits>

namespace doge
{
    struct Engine;

    struct StateMachine
    {
        fsm::StateType state;
        std::function<fsm::StateType(fsm::StateType, Entity, Engine&, DeltaTime)> transition;
        bool is_fixed_update = false;

        Event<fsm::StateType, Entity, Engine&, DeltaTime> on_entry;
        Event<fsm::StateType, Entity, Engine&, DeltaTime> on_exit;
    };
}