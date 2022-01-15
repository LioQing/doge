#include <doge/extensions/fsm/fsm.hpp>

#include <doge/core.hpp>
#include <doge/extensions/fsm/StateMachine.hpp>
#include <doge/extensions/fsm/StateMachines.hpp>

namespace doge
{
    void fsm::Enable(Engine& engine)
    {
        GameLoopFunctions glf;
        glf.update = Update;
        glf.fixed_update = FixedUpdate;
        
        engine.scenes.extensions.emplace("doge_fsm", glf);
    }

    void fsm::Disable(Engine& engine)
    {
        engine.scenes.extensions.erase("doge_fsm");
    }

    void fsm::ManualTransition(StateMachine& state_machine, fsm::State new_state, Entity entity, Engine& engine, DeltaTime dt, bool force_call_event)
    {
        if (!force_call_event && state_machine.state == new_state)
            return;
        
        state_machine.on_exit(state_machine.state, entity, engine, dt);
        state_machine.state = new_state;
        state_machine.on_entry(state_machine.state, entity, engine, dt);
    }

    void fsm::ManualUpdate(Engine& engine, DeltaTime dt, StateMachine::AutoUpdate auto_update)
    {
        for (auto [entity, fsm] : engine
            .Select<StateMachine>()
            .Where([auto_update](const Entity& _, const StateMachine& fsm)
            { return fsm.auto_update == auto_update; })
            .EntitiesAndComponents())
        {
            if (fsm.transition)
                ManualTransition(fsm, fsm.transition(fsm.state, entity, engine, dt), entity, engine, dt);
        }

        for (auto [entity, fsms] : engine.Select<StateMachines>().EntitiesAndComponents())
        {
            for (auto& fsm : fsms.state_machines)
            {
                if (fsm.transition)
                    ManualTransition(fsm, fsm.transition(fsm.state, entity, engine, dt), entity, engine, dt);
            }
        }
    }

    void fsm::Update(Engine& engine, DeltaTime dt)
    {
        ManualUpdate(engine, dt, StateMachine::AutoUpdate::Update);
    }

    void fsm::FixedUpdate(Engine& engine, DeltaTime dt)
    {
        ManualUpdate(engine, dt, StateMachine::AutoUpdate::FixedUpdate);
    }
}