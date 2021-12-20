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
        world = std::make_unique<b2World>(b2Vec2(0.f, 98.f));
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

                if (entity.HasComponent<AngularVelocity>())
                    entity.GetComponent<AngularVelocity>().angular_velocity = body->GetAngularVelocity();
            }
        }
    }

    void physics::Update(Engine& engine, DeltaTime dt)
    {
        auto CreateBody = [](const Entity& entity, const RigidBody& rgbd, const b2Shape* shape) -> b2Body*
        {
            b2BodyDef body_def;
            body_def.type = cast::ToB2BodyType(rgbd.type);

            auto pos = entity.GetIfHasComponentElseDefault<Position>().position;
            body_def.position.Set(pos.x, pos.y);

            body_def.angle = entity.GetIfHasComponentElseDefault<Rotation>().rotation;

            auto vel = entity.GetIfHasComponentElseDefault<Velocity>().velocity;
            body_def.linearVelocity.Set(vel.x, vel.y);

            body_def.angularVelocity = entity.GetIfHasComponentElseDefault<AngularVelocity>().angular_velocity;

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

                b2Body* body = CreateBody(entity, rgbd, &rect);
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

                b2Body* body = CreateBody(entity, rgbd, &convex);
                bodies.emplace(entity.id, body);
                rgbd.OnRemoval([&, entity](){ bodies.erase(entity.id); std::cout << "erased " << entity.id << " " << bodies.size() << std::endl; });
            }
        }

        // circle collider
        for (auto [entity, rgbd, coll] : engine.Select<RigidBody, CircleCollider>().EntitiesAndComponents())
        {
            if (bodies.find(entity.id) == bodies.end())
            {
                b2CircleShape circle;
                circle.m_radius = coll.radius;
                circle.m_p.Set(coll.origin.x, coll.origin.y);

                b2Body* body = CreateBody(entity, rgbd, &circle);
                bodies.emplace(entity.id, body);
            }
        }

        // edge collider
        for (auto [entity, rgbd, coll] : engine.Select<RigidBody, EdgeCollider>().EntitiesAndComponents())
        {
            if (bodies.find(entity.id) == bodies.end())
            {
                b2ChainShape chain;
                std::vector<b2Vec2> vertices;
                std::transform(coll.points.begin(), coll.points.end(), std::back_inserter(vertices), 
                [&](const Vec2f& v) { return cast::ToB2Vec2(v + coll.origin); });
                if (coll.is_loop)
                    chain.CreateLoop(vertices.data(), vertices.size());
                else
                    chain.CreateChain(vertices.data(), vertices.size(), vertices.front(), vertices.back());

                b2Body* body = CreateBody(entity, rgbd, &chain);
                bodies.emplace(entity.id, body);
            }
        }

        // rigid body only
        for (auto [entity, rgbd] : engine.Select<RigidBody>().EntitiesAndComponents())
        {
            auto body_itr = bodies.find(rgbd.entity_id);
            if (body_itr == bodies.end())
            {
                b2BodyDef body_def;
                body_def.type = cast::ToB2BodyType(rgbd.type);
                auto pos = entity.GetIfHasComponentElseDefault<Position>().position;
                body_def.position.Set(pos.x, pos.y);
                body_def.angle = entity.GetIfHasComponentElseDefault<Rotation>().rotation;
                auto vel = entity.GetIfHasComponentElseDefault<Velocity>().velocity;
                body_def.linearVelocity.Set(vel.x, vel.y);

                b2Body* body = world->CreateBody(&body_def);
                bodies.emplace(entity.id, body);
            }
            else
            {
                b2Body* body = body_itr->second;

                auto pos = body->GetPosition();
                if (entity.HasComponent<Position>())
                    pos = cast::ToB2Vec2(entity.GetComponent<Position>().position);

                auto angle = body->GetAngle();
                if (entity.HasComponent<Rotation>())
                    angle = entity.GetComponent<Rotation>().rotation;

                auto vel = body->GetLinearVelocity();
                if (entity.HasComponent<Velocity>())
                    vel = cast::ToB2Vec2(entity.GetComponent<Velocity>().velocity);

                auto angular_vel = body->GetAngularVelocity();
                if (entity.HasComponent<AngularVelocity>())
                    angular_vel = entity.GetComponent<AngularVelocity>().angular_velocity;

                body->SetTransform(pos, angle);
                body->SetLinearVelocity(vel);
                body->SetAngularVelocity(angular_vel);
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