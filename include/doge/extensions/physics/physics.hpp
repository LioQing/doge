#pragma once

#include <box2d/box2d.h>
#include <doge/core/Entity.hpp>
#include <doge/components/RigidBody.hpp>
#include <doge/extensions/physics/PhysicsBody.hpp>
#include <doge/extensions/physics/PhysicsCollider.hpp>
#include <doge/extensions/physics/PhysicsBodyInit.hpp>
#include <memory>
#include <map>
#include <array>
#include <vector>
#include <tuple>

namespace doge
{
    struct Engine;

    struct physics
    {
        using Body = PhysicsBody;
        using Collider = PhysicsCollider;
        using BodyInit = PhysicsBodyInit;

        physics(const physics&) = delete;
        static void Enable(Engine& engine);
        static void Disable(Engine& engine);

        static void SetGravity(const Vec2f& gravity);
        static const Vec2f& GetGravity();

        static void SetBodyInit(EntityID entity_id, const BodyInit& init_values);
        static Body GetBody(EntityID entity_id);

        static bool HasBody(EntityID entity_id);
        static bool HasCompoundCollider(EntityID entity_id);

        static Collider GetCollider(EntityID entity_id);
        static Collider GetCollider(EntityID entity_id, Collider::Type type, std::size_t index);

    private:

        static std::unique_ptr<b2World> world;
        static std::unordered_map<EntityID, b2Body*> bodies;
        static std::unordered_map<EntityID, std::array<std::vector<b2Fixture*>, Collider::Type::Count>> compound_fixtures;
        static std::unordered_map<EntityID, BodyInit> body_inits;
        static Vec2f gravity;

        static void Start(Engine& engine);
        static void Update(Engine& engine, DeltaTime dt);
        static void FixedUpdate(Engine& engine, DeltaTime dt);
        static void Finish(Engine& engine);
    };
}