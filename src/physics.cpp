#include <doge/extensions/physics.hpp>

#include <doge/core/Engine.hpp>

namespace doge
{
    std::unique_ptr<b2World> physics::world;

    void physics::Enable(Engine& engine)
    {
        engine.AddExtension("doge_box2d", Start, Update, FixedUpdate, Finish);
        world = std::make_unique<b2World>(b2Vec2_zero);
    }

    void physics::Disable(Engine& engine)
    {
        engine.EraseExtension("doge_box2d");
    }

    void physics::Start(Engine& engine)
    {
        world = std::make_unique<b2World>(b2Vec2_zero);
    }

    void physics::Update(Engine& engine, DeltaTime dt)
    {
        for (auto [rgbd] : engine.Select<RigidBody>().Components())
        {
            // if (bodies.find(rgbd.entity_id) == bodies.end())
            //     bodies.emplace(rgbd.entity_id, world->CreateBody());
        }
    }

    void physics::FixedUpdate(Engine& engine, DeltaTime dt)
    {
        world->Step(dt / 1000.f, 1, 1);
    }

    void physics::Finish(Engine& engine)
    {
        world.release();
    }

    void physics::SyncRigidBodies(Engine& engine)
    {
        
    }
}