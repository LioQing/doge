#pragma once

#include <box2d/box2d.h>
#include <doge/core/Entity.hpp>
#include <memory>
#include <unordered_map>

namespace doge
{
    struct Engine;

    struct physics
    {
        physics(const physics&) = delete;

        static std::unique_ptr<b2World> world;
        static std::unordered_map<EntityID, b2Body*> bodies;

        static void Enable(Engine& engine);
        static void Disable(Engine& engine);

    private:

        static void Start(Engine& engine);
        static void Update(Engine& engine, DeltaTime dt);
        static void FixedUpdate(Engine& engine, DeltaTime dt);
        static void Finish(Engine& engine);

        static void SyncRigidBodies(Engine& engine);
    };
}