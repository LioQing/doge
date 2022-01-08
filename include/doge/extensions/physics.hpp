#pragma once

#include <box2d/box2d.h>
#include <doge/core/Entity.hpp>
#include <doge/components/RigidBody.hpp>
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
        physics(const physics&) = delete;
        static void Enable(Engine& engine);
        static void Disable(Engine& engine);

    private:

        enum class FixtureType : std::uint8_t
        {
            Convex, Circle, Rectangle
        };

        static std::unique_ptr<b2World> world;
        static std::unordered_map<EntityID, b2Body*> bodies;
        static std::unordered_map<EntityID, std::array<std::vector<b2Fixture*>, 3>> compound_fixtures;
        static Vec2f gravity;

        static void Start(Engine& engine);
        static void Update(Engine& engine, DeltaTime dt);
        static void FixedUpdate(Engine& engine, DeltaTime dt);
        static void Finish(Engine& engine);
    };
}