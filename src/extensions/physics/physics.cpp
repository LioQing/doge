#include <doge/extensions/physics/physics.hpp>

#include <doge/core/Engine.hpp>
#include <doge/components.hpp>

namespace doge
{
    std::unique_ptr<b2World> physics::world;
    std::unordered_map<EntityID, b2Body*> physics::bodies;
    std::unordered_map<EntityID, std::array<std::vector<b2Fixture*>, physics::Collider::Type::Count>> physics::compound_fixtures;
    std::unordered_map<EntityID, physics::BodyInit> physics::body_inits;
    Vec2f physics::gravity = Vec2f(0.f, 0.98f);

    void physics::Enable(Engine& engine)
    {
        GameLoopFunctions glf;
        glf.start = Start;
        glf.update = Update;
        glf.fixed_update = FixedUpdate;
        glf.finish = Finish;
        
        engine.scenes.extensions.emplace("doge_box2d", glf);
        world = std::make_unique<b2World>(cast::ToB2Vec2(gravity));
    }

    void physics::Disable(Engine& engine)
    {
        engine.scenes.extensions.erase("doge_box2d");
    }

    void physics::SetBodyInit(EntityID entity_id, const BodyInit& init_values)
    {
        if (!body_inits.emplace(entity_id, init_values).second)
            throw std::invalid_argument(std::string("Failed to set BodyInit for Entity ") + std::to_string(entity_id));
    }

    physics::Body physics::GetBody(EntityID entity_id)
    {
        return Body(bodies.at(entity_id));
    }

    physics::Collider physics::GetCollider(EntityID entity_id)
    {
        return Collider(GetBody(entity_id).b2_body->GetFixtureList());
    }

    physics::Collider physics::GetCollider(EntityID entity_id, Collider::Type type, std::size_t index)
    {
        return Collider(compound_fixtures.at(entity_id).at(type).at(index));
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
        auto CreateBody = [&](const Entity& entity, const RigidBody& rgbd) -> b2Body*
        {
            b2BodyDef body_def;
            body_def.type = cast::ToB2BodyType(rgbd.type);

            auto pos = entity.GetIfHasComponentElseDefault<Position>().position;
            body_def.position = cast::ToB2Vec2(pos);

            body_def.angle = entity.GetIfHasComponentElseDefault<Rotation>().rotation;

            body_def.bullet = rgbd.continuous;
            body_def.awake = rgbd.awake;
            body_def.enabled = rgbd.enabled;

            b2Body* body_ptr = world->CreateBody(&body_def);

            if (body_inits.find(entity.id) != body_inits.end())
            {
                Body body(body_ptr);
                auto& init = body_inits.at(entity.id);

                body.SetTransform(init.position, init.rotation);
                body.SetVelocity(init.velocity);
                body.SetAngularVelocity(init.angular_velocity);
                body.SetDamping(init.damping);
                body.SetAngularDamping(init.angular_damping);
            }

            return body_ptr;
        };

        auto AddFixture = []<typename TComp>(b2Body* body, const TComp& coll_comp, const b2Shape* shape)
        {
            b2FixtureDef fixture_def;
            fixture_def.shape = shape;
            fixture_def.density = coll_comp.density;
            fixture_def.friction = coll_comp.friction;
            fixture_def.restitution = coll_comp.restitution;
            fixture_def.restitutionThreshold = coll_comp.restitution_threshold;
            fixture_def.isSensor = coll_comp.is_trigger;

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
            fixture->SetFriction(coll.friction);
            fixture->SetRestitution(coll.restitution);
            fixture->SetRestitutionThreshold(coll.restitution_threshold);
            fixture->SetSensor(coll.is_trigger);
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
            auto scale = global::GetScale(entity);
            circle.m_radius = coll.radius * scale.x;
            circle.m_p = cast::ToB2Vec2(-coll.origin * scale);
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
                coll.size.x * scale.x / 2.f, coll.size.y * scale.y / 2.f, 
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
                    compound_fixtures.emplace(entity, std::array<std::vector<b2Fixture*>, Collider::Type::Count>
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

                    fixtures = std::array<std::vector<b2Fixture*>, Collider::Type::Count>
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

                    fixtures.at(Collider::Type::Convex).at(i) = AddFixture(body, convex_coll, &convex).second;

                    convex_coll.apply_changes = false;
                }

                // circle sub collider
                for (auto i = 0; i < coll.circle_colliders.size(); ++i)
                {
                    auto& circle_coll = coll.circle_colliders.at(i);

                    b2CircleShape circle;
                    SyncCircle(circle, entity, circle_coll);

                    fixtures.at(Collider::Type::Circle).at(i) = AddFixture(body, circle_coll, &circle).second;

                    circle_coll.apply_changes = false;
                }

                // rectangle sub collider
                for (auto i = 0; i < coll.rectangle_colliders.size(); ++i)
                {
                    auto& rectangle_coll = coll.rectangle_colliders.at(i);

                    b2PolygonShape rect;
                    rect.SetAsBox(
                        rectangle_coll.size.x * scale.x / 2.f, rectangle_coll.size.y * scale.y / 2.f, 
                        cast::ToB2Vec2(rectangle_coll.origin), 
                        0
                    );

                    fixtures.at(Collider::Type::Rectangle).at(i) = AddFixture(body, rectangle_coll, &rect).second;

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
                    auto*& fixture = compound_fixtures.at(entity).at(Collider::Type::Convex).at(i);
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
                    auto*& fixture = compound_fixtures.at(entity).at(Collider::Type::Circle).at(i);
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
                    auto*& fixture = compound_fixtures.at(entity).at(Collider::Type::Rectangle).at(i);
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

                SaveBody(entity, rgbd, world->CreateBody(&body_def));
            }
            else
            {
                b2Body* body = body_itr->second;

                body->SetEnabled(rgbd.enabled);
                if (!rgbd.enabled)
                    continue;
                
                body->SetAwake(rgbd.awake);

                auto pos = body->GetPosition();
                if (entity.HasComponent<Position>())
                    pos = cast::ToB2Vec2(entity.GetComponent<Position>().position);

                auto angle = body->GetAngle();
                if (entity.HasComponent<Rotation>())
                    angle = entity.GetComponent<Rotation>().rotation;

                body->SetTransform(pos, angle);

                if (cast::ToB2BodyType(rgbd.type) != body->GetType())
                    body->SetType(cast::ToB2BodyType(rgbd.type));

                body->SetBullet(rgbd.continuous);
            }
        }

        // clear body inits
        body_inits.clear();
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
            }
        }
    }

    void physics::Finish(Engine& engine)
    {
        world.reset();
        bodies.clear();
    }
}