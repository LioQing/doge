#include <doge/extensions/fsm/fsm.hpp>

#include <doge/core.hpp>
#include <doge/extensions/fsm/StateMachine.hpp>

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

    void fsm::Update(Engine& engine, DeltaTime dt)
    {
        for (auto [entity, fsm] : engine
            .Select<StateMachine>()
            .Where([](const Entity& _, const StateMachine& fsm)
            { return !fsm.is_fixed_update; })
            .EntitiesAndComponents())
        {
            fsm::StateType new_state = fsm.transition(fsm.state, entity, engine, dt);

            if (new_state == fsm.state)
                continue;

            fsm.on_exit(fsm.state, entity, engine, dt);
            fsm.state = new_state;
            fsm.on_entry(fsm.state, entity, engine, dt);
        }
    }

    void fsm::FixedUpdate(Engine& engine, DeltaTime dt)
    {
        for (auto [entity, fsm] : engine
            .Select<StateMachine>()
            .Where([](const Entity& _, const StateMachine& fsm)
            { return fsm.is_fixed_update; })
            .EntitiesAndComponents())
        {
            fsm::StateType new_state = fsm.transition(fsm.state, entity, engine, dt);

            if (new_state == fsm.state)
                continue;

            fsm.on_exit(fsm.state, entity, engine, dt);
            fsm.state = new_state;
            fsm.on_entry(fsm.state, entity, engine, dt);
        }
    }
}