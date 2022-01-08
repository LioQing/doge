#include <doge/extensions/physics.hpp>

#include <doge/core/Engine.hpp>
#include <doge/components.hpp>

namespace doge
{
    std::unique_ptr<b2World> physics::world;
    std::unordered_map<EntityID, b2Body*> physics::bodies;
    std::unordered_map<EntityID, std::array<std::vector<b2Fixture*>, 3>> physics::compound_fixtures;
    Vec2f physics::gravity = Vec2f(0.f, 98.f);

    void physics::Enable(Engine& engine)
    {
        GameLoopFunctions glf;
        glf.start = Start;
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

    void physics::Update(Engine& engine, DeltaTime dt)
    {
        // helper functions
        auto CreateBody = [](const Entity& entity, const RigidBody& rgbd) -> b2Body*
        {
            b2BodyDef body_def;
            body_def.type = cast::ToB2BodyType(rgbd.type);

            auto pos = entity.GetIfHasComponentElseDefault<Position>().position;
            body_def.position = cast::ToB2Vec2(pos);

            body_def.angle = entity.GetIfHasComponentElseDefault<Rotation>().rotation;

            auto vel = entity.GetIfHasComponentElseDefault<Velocity>().velocity;
            body_def.linearVelocity = cast::ToB2Vec2(vel);

            body_def.angularVelocity = entity.GetIfHasComponentElseDefault<AngularVelocity>().angular_velocity;

            b2Body* body = world->CreateBody(&body_def);
            return body;
        };

        auto AddFixture = []<typename TComp>(b2Body* body, const TComp& coll_comp, const b2Shape* shape)
        {
            b2FixtureDef fixture_def;
            fixture_def.shape = shape;
            fixture_def.density = coll_comp.density;
            fixture_def.restitution = coll_comp.restitution;
            fixture_def.friction = coll_comp.friction;

            b2Fixture* fixture = body->CreateFixture(&fixture_def);
            return std::make_pair(body, fixture);
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

        auto SyncFixture = []<typename TComp>(b2Fixture* fixture, const TComp& coll)
        {
            fixture->SetDensity(coll.density);
            fixture->SetRestitution(coll.restitution);
            fixture->SetFriction(coll.friction);
        };

        // convex collider
        auto SyncConvex = [](b2PolygonShape& convex, const Entity& entity, const ConvexCollider& coll)
        {
            auto scale = global::GetScale(entity);
            std::vector<b2Vec2> vertices;
            std::transform(coll.points.begin(), coll.points.end(), std::back_inserter(vertices), 
            [&](const Vec2f& v) { return cast::ToB2Vec2((v - coll.origin) * scale); });
            convex.Set(vertices.data(), vertices.size());
        };

        for (auto [entity, rgbd, coll] : engine.Select<RigidBody, ConvexCollider>().EntitiesAndComponents())
        {
            auto body_itr = bodies.find(entity.id);
            if (body_itr == bodies.end())
            {
                b2PolygonShape convex;
                SyncConvex(convex, entity, coll);
                AddFixture(SaveBody(entity, rgbd, CreateBody(entity, rgbd))->second, coll, &convex);
            }
            else if (coll.apply_changes)
            {
                auto* body = body_itr->second;
                auto* fixture = body->GetFixtureList();
                auto* shape = static_cast<b2PolygonShape*>(fixture->GetShape());

                if (shape->m_count != coll.points.size())
                {
                    body->DestroyFixture(fixture);
                    b2PolygonShape convex;
                    SyncConvex(convex, entity, coll);
                    AddFixture(body, coll, &convex);
                }
                else
                {
                    SyncFixture(fixture, coll);
                    SyncConvex(*shape, entity, coll);
                }

                coll.apply_changes = false;
            }
        }

        // circle collider
        auto SyncCircle = [](b2CircleShape& circle, const Entity& entity, const CircleCollider& coll)
        {
            circle.m_radius = cast::ToB2Length(coll.radius);
            circle.m_p = cast::ToB2Vec2(-coll.origin);
        };

        for (auto [entity, rgbd, coll] : engine.Select<RigidBody, CircleCollider>().EntitiesAndComponents())
        {
            auto body_itr = bodies.find(entity.id);
            if (body_itr == bodies.end())
            {
                b2CircleShape circle;
                SyncCircle(circle, entity, coll);

                AddFixture(SaveBody(entity, rgbd, CreateBody(entity, rgbd))->second, coll, &circle);
            }
            else if (coll.apply_changes)
            {
                auto* body = body_itr->second;
                auto* fixture = body->GetFixtureList();
                auto* shape = static_cast<b2CircleShape*>(fixture->GetShape());

                SyncFixture(fixture, coll);
                SyncCircle(*shape, entity, coll);

                coll.apply_changes = false;
            }
        }

        // rectangle collider
        auto SyncRectangle = [](b2PolygonShape& rect, const Entity& entity, const RectangleCollider& coll)
        {
            auto scale = global::GetScale(entity);
            rect.SetAsBox(
                cast::ToB2Length(coll.size.x * scale.x) / 2.f, cast::ToB2Length(coll.size.y * scale.y) / 2.f, 
                cast::ToB2Vec2(-coll.origin), 
                0
            );
        };

        for (auto [entity, rgbd, coll] : engine.Select<RigidBody, RectangleCollider>().EntitiesAndComponents())
        {
            auto body_itr = bodies.find(entity.id);
            if (body_itr == bodies.end())
            {
                b2PolygonShape rect;
                SyncRectangle(rect, entity, coll);

                AddFixture(SaveBody(entity, rgbd, CreateBody(entity, rgbd))->second, coll, &rect);
            }
            else if (coll.apply_changes)
            {
                auto* body = body_itr->second;
                auto* fixture = body->GetFixtureList();
                auto* shape = static_cast<b2PolygonShape*>(fixture->GetShape());

                SyncFixture(fixture, coll);
                SyncRectangle(*shape, entity, coll);

                coll.apply_changes = false;
            }
        }

        // edge collider
        auto SyncEdge = [](b2ChainShape& chain, const Entity& entity, const EdgeCollider& coll)
        {
            auto scale = global::GetScale(entity);
            std::vector<b2Vec2> vertices;
            std::transform(coll.points.begin(), coll.points.end(), std::back_inserter(vertices), 
            [&](const Vec2f& v) { return cast::ToB2Vec2((v - coll.origin) * scale); });
            if (coll.is_loop)
                chain.CreateLoop(vertices.data(), vertices.size());
            else
                chain.CreateChain(vertices.data(), vertices.size(), vertices.front(), vertices.back());
        };

        for (auto [entity, rgbd, coll] : engine.Select<RigidBody, EdgeCollider>().EntitiesAndComponents())
        {
            auto body_itr = bodies.find(entity.id);
            if (body_itr == bodies.end())
            {
                b2ChainShape chain;
                SyncEdge(chain, entity, coll);

                AddFixture(SaveBody(entity, rgbd, CreateBody(entity, rgbd))->second, coll, &chain);
            }
            else if (coll.apply_changes)
            {
                auto* body = body_itr->second;
                auto* fixture = body->GetFixtureList();
                auto* shape = static_cast<b2ChainShape*>(fixture->GetShape());

                if (shape->m_count != coll.points.size())
                {
                    body->DestroyFixture(fixture);
                    b2ChainShape chain;
                    SyncEdge(chain, entity, coll);
                    AddFixture(body, coll, &chain);
                }
                else
                {
                    SyncFixture(fixture, coll);
                    SyncEdge(*shape, entity, coll);
                }

                coll.apply_changes = false;
            }
        }

        // compound collider
        for (auto [entity, rgbd, coll] : engine.Select<RigidBody, CompoundCollider>().EntitiesAndComponents())
        {
            auto body_itr = bodies.find(entity.id);
            if (body_itr == bodies.end() || coll.apply_changes)
            {
                auto scale = global::GetScale(entity);

                if (body_itr == bodies.end())
                {
                    body_itr = SaveBody(entity, rgbd, CreateBody(entity, rgbd));
                    compound_fixtures.emplace(entity, std::array<std::vector<b2Fixture*>, 3>
                    { 
                        std::vector<b2Fixture*>(coll.convex_colliders.size()), 
                        std::vector<b2Fixture*>(coll.circle_colliders.size()), 
                        std::vector<b2Fixture*>(coll.rectangle_colliders.size()) 
                    });
                }

                auto& fixtures = compound_fixtures.at(entity);
                auto* body = body_itr->second;

                // clear fixture
                if (coll.apply_changes)
                {
                    auto* fixture = body->GetFixtureList();
                    b2Fixture* next = nullptr;
                    while (fixture != nullptr)
                    {
                        next = fixture->GetNext();
                        body->DestroyFixture(fixture);
                        fixture = next;
                    }

                    fixtures = std::array<std::vector<b2Fixture*>, 3>
                    { 
                        std::vector<b2Fixture*>(coll.convex_colliders.size()), 
                        std::vector<b2Fixture*>(coll.circle_colliders.size()), 
                        std::vector<b2Fixture*>(coll.rectangle_colliders.size()) 
                    };

                    coll.apply_changes = false;
                }

                // convex sub collider
                for (auto i = 0; i < coll.convex_colliders.size(); ++i)
                {
                    auto& convex_coll = coll.convex_colliders.at(i);

                    b2PolygonShape convex;
                    SyncConvex(convex, entity, convex_coll);

                    fixtures.at(static_cast<uint8_t>(FixtureType::Convex)).at(i) = AddFixture(body, convex_coll, &convex).second;

                    convex_coll.apply_changes = false;
                }

                // circle sub collider
                for (auto i = 0; i < coll.circle_colliders.size(); ++i)
                {
                    auto& circle_coll = coll.circle_colliders.at(i);

                    b2CircleShape circle;
                    SyncCircle(circle, entity, circle_coll);

                    fixtures.at(static_cast<uint8_t>(FixtureType::Circle)).at(i) = AddFixture(body, circle_coll, &circle).second;

                    circle_coll.apply_changes = false;
                }

                // rectangle sub collider
                for (auto i = 0; i < coll.rectangle_colliders.size(); ++i)
                {
                    auto& rectangle_coll = coll.rectangle_colliders.at(i);

                    b2PolygonShape rect;
                    rect.SetAsBox(
                        cast::ToB2Length(rectangle_coll.size.x * scale.x) / 2.f, cast::ToB2Length(rectangle_coll.size.y * scale.y) / 2.f, 
                        cast::ToB2Vec2(rectangle_coll.origin), 
                        0
                    );

                    fixtures.at(static_cast<uint8_t>(FixtureType::Rectangle)).at(i) = AddFixture(body, rectangle_coll, &rect).second;

                    rectangle_coll.apply_changes = false;
                }
            }
            else
            {
                auto* body = body_itr->second;
                
                // convex sub collider
                for (auto i = 0; i < coll.convex_colliders.size(); ++i)
                {
                    auto& convex_coll = coll.convex_colliders.at(i);

                    if (!convex_coll.apply_changes)
                        continue;

                    auto* body = body_itr->second;
                    auto*& fixture = compound_fixtures.at(entity).at(static_cast<uint8_t>(FixtureType::Convex)).at(i);
                    auto* shape = static_cast<b2PolygonShape*>(fixture->GetShape());

                    if (shape->m_count != convex_coll.points.size())
                    {
                        body->DestroyFixture(fixture);
                        b2PolygonShape convex;
                        SyncConvex(convex, entity, convex_coll);
                        fixture = AddFixture(body, convex_coll, &convex).second;
                    }
                    else
                    {
                        SyncFixture(fixture, convex_coll);
                        SyncConvex(*shape, entity, convex_coll);
                    }

                    coll.apply_changes = false;
                }

                // circle sub collider
                for (auto i = 0; i < coll.circle_colliders.size(); ++i)
                {
                    auto& circle_coll = coll.circle_colliders.at(i);

                    if (!circle_coll.apply_changes)
                        continue;

                    auto* body = body_itr->second;
                    auto*& fixture = compound_fixtures.at(entity).at(static_cast<uint8_t>(FixtureType::Circle)).at(i);
                    auto* shape = static_cast<b2CircleShape*>(fixture->GetShape());

                    SyncFixture(fixture, circle_coll);
                    SyncCircle(*shape, entity, circle_coll);

                    circle_coll.apply_changes = false;
                }

                // rectangle sub collider
                for (auto i = 0; i < coll.rectangle_colliders.size(); ++i)
                {
                    auto& rectangle_coll = coll.rectangle_colliders.at(i);

                    if (!rectangle_coll.apply_changes)
                        continue;

                    auto* body = body_itr->second;
                    auto*& fixture = compound_fixtures.at(entity).at(static_cast<uint8_t>(FixtureType::Rectangle)).at(i);
                    auto* shape = static_cast<b2PolygonShape*>(fixture->GetShape());

                    SyncFixture(fixture, rectangle_coll);
                    SyncRectangle(*shape, entity, rectangle_coll);

                    rectangle_coll.apply_changes = false;
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
                body_def.type = cast::ToB2BodyType(rgbd.type);
                auto pos = entity.GetIfHasComponentElseDefault<Position>().position;
                body_def.position = cast::ToB2Vec2(pos);
                body_def.angle = entity.GetIfHasComponentElseDefault<Rotation>().rotation;
                auto vel = entity.GetIfHasComponentElseDefault<Velocity>().velocity;
                body_def.linearVelocity = cast::ToB2Vec2(vel);

                SaveBody(entity, rgbd, world->CreateBody(&body_def));
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

    void physics::Finish(Engine& engine)
    {
        world.reset();
        bodies.clear();
    }
}