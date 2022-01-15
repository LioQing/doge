#pragma once

#include <doge/extensions/fsm/fsm.hpp>
#include <doge/extensions/fsm/StateMachineAutoUpdate.hpp>
#include <doge/utils.hpp>
#include <functional>

namespace doge
{
    struct Engine;
    struct Entity;

    struct StateMachine
    {
        using AutoUpdate = StateMachineAutoUpdate;

        fsm::State state;
        std::function<fsm::State(fsm::State, Entity, Engine&, DeltaTime)> transition;
        AutoUpdate auto_update = AutoUpdate::Update;

        Event<fsm::State, Entity, Engine&, DeltaTime> on_entry;
        Event<fsm::State, Entity, Engine&, DeltaTime> on_exit;
    };
}