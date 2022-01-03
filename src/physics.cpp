#include <doge/extensions/physics.hpp>

#include <doge/core/Engine.hpp>
#include <doge/components.hpp>

namespace doge
{
    std::unique_ptr<b2World> physics::world;
    std::unordered_map<EntityID, b2Body*> physics::bodies;
    Vec2f physics::gravity = Vec2f(0.f, 98.f);

    void physics::Enable(Engine& engine)
    {
        GameLoopFunctions glf;
        glf.start = Start;
        glf.early_update = EarlyUpdate;
        glf.update = Update;
        glf.fixed_update = FixedUpdate;
        glf.finish = Finish;
        
        engine.AddExtension("doge_box2d", glf);
        world = std::make_unique<b2World>(cast::ToB2Vec2(gravity));
    }

    void physics::Disable(Engine& engine)
    {
        engine.EraseExtension("doge_box2d");
    }

    void physics::Start(Engine& engine)
    {
        if (world)
            world.reset();
        world = std::make_unique<b2World>(cast::ToB2Vec2(gravity));
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
                    entity.GetComponent<Position>()._NoModify_Get() = cast::FromB2Vec2(body->GetPosition());

                if (entity.HasComponent<Rotation>())
                    entity.GetComponent<Rotation>()._NoModify_Get() = body->GetAngle();

                if (entity.HasComponent<Velocity>())
                    entity.GetComponent<Velocity>()._NoModify_Get() = cast::FromB2Vec2(body->GetLinearVelocity());

                if (entity.HasComponent<AngularVelocity>())
                    entity.GetComponent<AngularVelocity>()._NoModify_Get() = body->GetAngularVelocity();
            }
        }
    }

    void physics::Update(Engine& engine, DeltaTime dt)
    {
        // helper functions
        auto CreateBody = [](const Entity& entity, const RigidBody& rgbd) -> b2Body*
        {
            b2BodyDef body_def;
            body_def.type = cast::ToB2BodyType(rgbd.GetType());

            auto pos = entity.GetIfHasComponentElseDefault<Position>().Get();
            body_def.position = cast::ToB2Vec2(pos);

            body_def.angle = entity.GetIfHasComponentElseDefault<Rotation>().Get();

            auto vel = entity.GetIfHasComponentElseDefault<Velocity>().Get();
            body_def.linearVelocity = cast::ToB2Vec2(vel);

            body_def.angularVelocity = entity.GetIfHasComponentElseDefault<AngularVelocity>().Get();

            b2Body* body = world->CreateBody(&body_def);
            return body;
        };

        auto AddFixture = []<typename TComp>(b2Body* body, const TComp& coll_comp, const b2Shape* shape)
        {
            b2FixtureDef fixture_def;
            fixture_def.shape = shape;
            fixture_def.density = coll_comp.GetDensity();
            fixture_def.restitution = coll_comp.GetRestitution();
            fixture_def.friction = coll_comp.GetFriction();

            body->CreateFixture(&fixture_def);
            return body;
        };

        auto SaveBody = [&](EntityID eid, const Component<RigidBody>& rgbd, b2Body* body)
        {
            auto [body_itr, success] = bodies.emplace(eid, body);

            rgbd.OnRemoval([&, eid]()
            {
                if (world)
                    world->DestroyBody(bodies.at(eid));
                bodies.erase(eid);
            });

            return body_itr;
        };

        // convex collider
        for (auto [entity, rgbd, coll] : engine.Select<RigidBody, ConvexCollider>().EntitiesAndComponents())
        {
            if (bodies.find(entity.id) == bodies.end())
            {
                auto scale = global::GetScale(entity);
                b2PolygonShape convex;
                std::vector<b2Vec2> vertices;
                std::transform(coll.GetPoints().begin(), coll.GetPoints().end(), std::back_inserter(vertices), 
                [&](const Vec2f& v) { return cast::ToB2Vec2((v - coll.GetOrigin()) * scale); });
                convex.Set(vertices.data(), vertices.size());

                SaveBody(entity, rgbd, AddFixture(CreateBody(entity, rgbd), coll, &convex));
            }
        }

        // circle collider
        for (auto [entity, rgbd, coll] : engine.Select<RigidBody, CircleCollider>().EntitiesAndComponents())
        {
            if (bodies.find(entity.id) == bodies.end())
            {
                b2CircleShape circle;
                circle.m_radius = cast::ToB2Length(coll.GetRadius());
                circle.m_p = cast::ToB2Vec2(-coll.GetOrigin());

                SaveBody(entity, rgbd, AddFixture(CreateBody(entity, rgbd), coll, &circle));
            }
        }

        // rectangle collider
        for (auto [entity, rgbd, coll] : engine.Select<RigidBody, RectangleCollider>().EntitiesAndComponents())
        {
            if (bodies.find(entity.id) == bodies.end())
            {
                auto scale = global::GetScale(entity);
                b2PolygonShape rect;
                rect.SetAsBox(
                    cast::ToB2Length(coll.GetSize().x * scale.x) / 2.f, cast::ToB2Length(coll.GetSize().y * scale.y) / 2.f, 
                    cast::ToB2Vec2(coll.GetOrigin()), 
                    0
                );

                SaveBody(entity, rgbd, AddFixture(CreateBody(entity, rgbd), coll, &rect));
            }
        }

        // edge collider
        for (auto [entity, rgbd, coll] : engine.Select<RigidBody, EdgeCollider>().EntitiesAndComponents())
        {
            if (bodies.find(entity.id) == bodies.end())
            {
                auto scale = global::GetScale(entity);
                b2ChainShape chain;
                std::vector<b2Vec2> vertices;
                std::transform(coll.GetPoints().begin(), coll.GetPoints().end(), std::back_inserter(vertices), 
                [&](const Vec2f& v) { return cast::ToB2Vec2((v - coll.GetOrigin()) * scale); });
                if (coll.GetIsLoop())
                    chain.CreateLoop(vertices.data(), vertices.size());
                else
                    chain.CreateChain(vertices.data(), vertices.size(), vertices.front(), vertices.back());

                SaveBody(entity, rgbd, AddFixture(CreateBody(entity, rgbd), coll, &chain));
            }
        }

        // compound collider
        for (auto [entity, rgbd, coll] : engine.Select<RigidBody, CompoundCollider>().EntitiesAndComponents())
        {
            if (bodies.find(entity.id) == bodies.end())
            {
                auto scale = global::GetScale(entity);

                auto* body = SaveBody(entity, rgbd, CreateBody(entity, rgbd))->second;

                // convex sub collider
                for (auto& convex_collider : coll.GetConvexColliders())
                {
                    b2PolygonShape convex;
                    std::vector<b2Vec2> vertices;
                    std::transform(convex_collider.GetPoints().begin(), convex_collider.GetPoints().end(), std::back_inserter(vertices), 
                    [&](const Vec2f& v) { return cast::ToB2Vec2((v - convex_collider.GetOrigin()) * scale); });
                    convex.Set(vertices.data(), vertices.size());

                    AddFixture(body, convex_collider, &convex);
                }

                // circle sub collider
                for (auto& circle_collider : coll.GetCircleColliders())
                {
                    b2CircleShape circle;
                    circle.m_radius = cast::ToB2Length(circle_collider.GetRadius());
                    circle.m_p = cast::ToB2Vec2(-circle_collider.GetOrigin());

                    AddFixture(body, circle_collider, &circle);
                }

                // rectangle sub collider
                for (auto& rectangle_collider : coll.GetRectangleColliders())
                {
                    b2PolygonShape rect;
                    rect.SetAsBox(
                        cast::ToB2Length(rectangle_collider.GetSize().x * scale.x) / 2.f, cast::ToB2Length(rectangle_collider.GetSize().y * scale.y) / 2.f, 
                        cast::ToB2Vec2(rectangle_collider.GetOrigin()), 
                        0
                    );

                    AddFixture(body, rectangle_collider, &rect);
                }
            }
        }

        // rigid body only
        for (auto [entity, rgbd] : engine.Select<RigidBody>().EntitiesAndComponents())
        {
            auto body_itr = bodies.find(rgbd.entity_id);
            if (body_itr == bodies.end())
            {
                b2BodyDef body_def;
                body_def.type = cast::ToB2BodyType(rgbd.GetType());
                auto pos = entity.GetIfHasComponentElseDefault<Position>().Get();
                body_def.position = cast::ToB2Vec2(pos);
                body_def.angle = entity.GetIfHasComponentElseDefault<Rotation>().Get();
                auto vel = entity.GetIfHasComponentElseDefault<Velocity>().Get();
                body_def.linearVelocity = cast::ToB2Vec2(vel);

                SaveBody(entity, rgbd, world->CreateBody(&body_def));
            }
            else
            {
                b2Body* body = body_itr->second;

                auto pos = body->GetPosition();
                if (entity.HasComponent<Position>())
                    pos = cast::ToB2Vec2(entity.GetComponent<Position>().Get());

                auto angle = body->GetAngle();
                if (entity.HasComponent<Rotation>())
                    angle = entity.GetComponent<Rotation>().Get();

                auto vel = body->GetLinearVelocity();
                if (entity.HasComponent<Velocity>())
                    vel = cast::ToB2Vec2(entity.GetComponent<Velocity>().Get());

                auto angular_vel = body->GetAngularVelocity();
                if (entity.HasComponent<AngularVelocity>())
                    angular_vel = entity.GetComponent<AngularVelocity>().Get();

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
        world.reset();
        bodies.clear();
    }
}