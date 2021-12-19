#include <doge/extensions/physics.hpp>

#include <doge/core/Engine.hpp>

namespace doge
{
    std::unique_ptr<b2World> physics::world;
    std::unordered_map<EntityID, b2Body*> physics::bodies;

    void physics::Enable(Engine& engine)
    {
        GameLoopFunctions glf;
        glf.start = Start;
        glf.early_update = EarlyUpdate;
        glf.update = Update;
        glf.fixed_update = FixedUpdate;
        glf.finish = Finish;
        
        engine.AddExtension("doge_box2d", glf);
    }

    void physics::Disable(Engine& engine)
    {
        engine.EraseExtension("doge_box2d");
    }

    void physics::Start(Engine& engine)
    {
        world = std::make_unique<b2World>(b2Vec2(0.f, 40.f));
    }

    void physics::EarlyUpdate(Engine& engine, DeltaTime dt)
    {
        for (auto [entity, rgbd] : engine.Select<RigidBody>().EntitiesAndComponents())
        {
            auto body_itr = bodies.find(rgbd.entity_id);
            if (body_itr != bodies.end())
            {
                b2Body* body = body_itr->second;

                if (entity.HasComponent<Position>())
                    entity.GetComponent<Position>().position = cast::FromB2Vec2(body->GetPosition());

                if (entity.HasComponent<Rotation>())
                    entity.GetComponent<Rotation>().rotation = body->GetAngle();

                if (entity.HasComponent<Velocity>())
                    entity.GetComponent<Velocity>().velocity = cast::FromB2Vec2(body->GetLinearVelocity());
            }
        }
    }

    void physics::Update(Engine& engine, DeltaTime dt)
    {
        auto ToBody = [](const Entity& entity, const RigidBody& rgbd, const b2Shape* shape) -> b2Body*
        {
            b2BodyDef body_def;
            body_def.type = cast::ToB2BodyType(rgbd.type);
            auto pos = entity.GetIfHasComponentElseDefault<Position>().position;
            body_def.position.Set(pos.x, pos.y);
            body_def.angle = entity.GetIfHasComponentElseDefault<Rotation>().rotation;
            auto vel = entity.GetIfHasComponentElseDefault<Velocity>().velocity;
            body_def.linearVelocity.Set(vel.x, vel.y);

            b2FixtureDef fixture_def;
            fixture_def.shape = shape;
            fixture_def.density = rgbd.density;
            fixture_def.restitution = rgbd.restitution;
            fixture_def.friction = rgbd.friction;

            b2Body* body = world->CreateBody(&body_def);
            body->CreateFixture(&fixture_def);
            return body;
        };

        // rectangle collider
        for (auto [entity, rgbd, coll] : engine.Select<RigidBody, RectangleCollider>().EntitiesAndComponents())
        {
            if (bodies.find(entity.id) == bodies.end())
            {
                b2PolygonShape rect;
                rect.SetAsBox(
                    coll.size.x / 2.f, coll.size.y / 2.f, 
                    cast::ToB2Vec2(coll.origin), 
                    0
                );

                auto* body = ToBody(entity, rgbd, &rect);
                bodies.emplace(entity.id, body);
            }
        }

        // convex collider
        for (auto [entity, rgbd, coll] : engine.Select<RigidBody, ConvexCollider>().EntitiesAndComponents())
        {
            if (bodies.find(entity.id) == bodies.end())
            {
                b2PolygonShape convex;
                std::vector<b2Vec2> vertices;
                std::transform(coll.points.begin(), coll.points.end(), std::back_inserter(vertices), 
                [&](const Vec2f& v) { return cast::ToB2Vec2(v + coll.origin); });
                convex.Set(vertices.data(), vertices.size());

                auto* body = ToBody(entity, rgbd, &convex);
                bodies.emplace(entity.id, body);
            }
        }

        // circle collider
        for (auto [entity, rgbd, coll] : engine.Select<RigidBody, CircleCollider>().EntitiesAndComponents())
        {
            if (bodies.find(entity.id) == bodies.end())
            {
                b2CircleShape circle;
                circle.m_radius = coll.radius;
                circle.m_p = cast::ToB2Vec2(coll.origin);

                auto* body = ToBody(entity, rgbd, &circle);
                bodies.emplace(entity.id, body);
            }
        }
    }

    void physics::FixedUpdate(Engine& engine, DeltaTime dt)
    {
        world->Step(dt / 1000.f, 1, 1);
    }

    void physics::Finish(Engine& engine)
    {
        world.release();
        bodies.clear();
    }
}