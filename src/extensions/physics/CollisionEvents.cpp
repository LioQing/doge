#include <doge/extensions/physics/CollisionEvents.hpp>

#include <doge/extensions/physics/Physics.hpp>
#include <doge/extensions/physics/RigidBody.hpp>
#include <doge/core/Component.hpp>

namespace doge::physics
{
    CollisionEvents::CollisionEvents(Physics& physics) : physics(physics) 
    {
        on_collision_began += [&](Entity a, Entity b)
        {
            a.GetComponent<RigidBody>().on_collision_began(b);
            b.GetComponent<RigidBody>().on_collision_began(a);
        };

        on_collision_ended += [&](Entity a, Entity b)
        {
            a.GetComponent<RigidBody>().on_collision_ended(b);
            b.GetComponent<RigidBody>().on_collision_ended(a);
        };

        on_collision_presolve += [&](Entity a, Entity b)
        {
            a.GetComponent<RigidBody>().on_collision_presolve(b);
            b.GetComponent<RigidBody>().on_collision_presolve(a);
        };

        on_collision_postsolve += [&](Entity a, Entity b)
        {
            a.GetComponent<RigidBody>().on_collision_postsolve(b);
            b.GetComponent<RigidBody>().on_collision_postsolve(a);
        };
    }

    void CollisionEvents::BeginContact(b2Contact* contact)
    {
        on_collision_began(
            physics.FindEntityByB2Fixture(contact->GetFixtureA()),
            physics.FindEntityByB2Fixture(contact->GetFixtureB())
        );
    }

    void CollisionEvents::EndContact(b2Contact* contact)
    {
        on_collision_ended(
            physics.FindEntityByB2Fixture(contact->GetFixtureA()),
            physics.FindEntityByB2Fixture(contact->GetFixtureB())
        );
    }

    void CollisionEvents::PreSolve(b2Contact* contact, const b2Manifold* old_manifold)
    {
        on_collision_presolve(
            physics.FindEntityByB2Fixture(contact->GetFixtureA()),
            physics.FindEntityByB2Fixture(contact->GetFixtureB())
        );
    }

    void CollisionEvents::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
    {
        on_collision_postsolve(
            physics.FindEntityByB2Fixture(contact->GetFixtureA()),
            physics.FindEntityByB2Fixture(contact->GetFixtureB())
        );
    }
}