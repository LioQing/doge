#pragma once

#include <box2d/box2d.h>
#include <doge/core/Entity.hpp>
#include <doge/extensions/physics/RigidBody.hpp>
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

    struct Physics
    {
        using Body = PhysicsBody;
        using Collider = PhysicsCollider;
        using BodyInit = PhysicsBodyInit;
        using FixtureKey = std::tuple<EntityID, Collider::Type, std::size_t>; // eid, fixture type, index

        Physics(Engine& engine);
        ~Physics();

        void SetGravity(const Vec2f& gravity);
        const Vec2f& GetGravity() const;

        void SetBodyInit(EntityID entity_id, const BodyInit& init_values);
        Body GetBody(EntityID entity_id);
        const Body GetBody(EntityID entity_id) const;

        bool HasBody(EntityID entity_id) const;
        bool HasCompoundCollider(EntityID entity_id) const;

        Collider GetCollider(EntityID entity_id);
        Collider GetCollider(EntityID entity_id, Collider::Type type, std::size_t index);

        const Collider GetCollider(EntityID entity_id) const;
        const Collider GetCollider(EntityID entity_id, Collider::Type type, std::size_t index) const;

        static Vec2f FromB2Vec2(const b2Vec2& v);
        static b2Vec2 ToB2Vec2(const Vec2f& v);

        static b2BodyType ToB2BodyType(RigidBody::Type type);
        static RigidBody::Type FromB2BodyType(b2BodyType type);

    private:

        Engine& engine;

        std::unique_ptr<b2World> world;
        std::unordered_map<EntityID, b2Body*> bodies;
        std::unordered_map<EntityID, BodyInit> body_inits;
        std::map<FixtureKey, b2Fixture*> fixtures;
        Vec2f gravity = Vec2f(0, 9.8f);

        void Start(Engine& engine);
        void Update(Engine& engine, DeltaTime dt);
        void FixedUpdate(Engine& engine, DeltaTime dt);
        void Finish(Engine& engine);
    };
}