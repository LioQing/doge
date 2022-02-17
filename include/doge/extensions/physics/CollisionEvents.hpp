#pragma once

#include <box2d/box2d.h>
#include <doge/utils/lev.hpp>
#include <doge/core/Entity.hpp>

namespace doge::physics
{
    struct Physics;

    struct CollisionEvents : b2ContactListener
    {
        Event<Entity, Entity> on_collision_began;
        Event<Entity, Entity> on_collision_ended;
        Event<Entity, Entity> on_collision_presolve;
        Event<Entity, Entity> on_collision_postsolve;

        CollisionEvents(Physics& physics);

    private:

        Physics& physics;

        void BeginContact(b2Contact* contact) override;
        void EndContact(b2Contact* contact) override;
        void PreSolve(b2Contact* contact, const b2Manifold* old_manifold) override;
        void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override;
    };
}