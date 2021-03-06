#pragma once

#include <box2d/box2d.h>
#include <doge/core/Entity.hpp>
#include <doge/extensions/physics/RigidBody.hpp>
#include <doge/extensions/physics/CollisionEvents.hpp>
#include <doge/extensions/physics/Body.hpp>
#include <doge/extensions/physics/Collider.hpp>
#include <doge/extensions/physics/BodyInit.hpp>
#include <memory>
#include <map>
#include <array>
#include <vector>
#include <tuple>

namespace doge
{
    struct Engine;

    namespace physics
    {
        struct Physics
        {
            using FixtureKey = std::tuple<EntityID, Collider::Type, std::size_t>; // eid, fixture type, index
            
            CollisionEvents collision_events;

            Physics(Engine& engine);
            ~Physics();

            void SetPaused(bool paused);
            bool IsPaused() const;

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

            Entity FindEntityByB2Body(const b2Body* body) const;
            Entity FindEntityByBody(const Body& body) const;

            Entity FindEntityByB2Fixture(const b2Fixture* fixture) const;
            Entity FindEntityByCollider(const Collider& collider) const;
            
            static Vec2f FromB2Vec2(const b2Vec2& v);
            static b2Vec2 ToB2Vec2(const Vec2f& v);

            static b2BodyType ToB2BodyType(RigidBody::Type type);
            static RigidBody::Type FromB2BodyType(b2BodyType type);

        private:

            Engine& engine;
            bool paused = false;

            std::unique_ptr<b2World> world;
            std::unordered_map<EntityID, b2Body*> bodies;
            std::unordered_map<EntityID, BodyInit> body_inits;
            std::map<FixtureKey, b2Fixture*> fixtures;
            Vec2f gravity = Vec2f(0, 9.8f);

            void Start(Engine& engine);
            void LateUpdate(Engine& engine, DeltaTime dt);
            void FixedUpdate(Engine& engine, DeltaTime dt);
            void Finish(Engine& engine);

            friend struct CollisionEvents;
        };
    }
}