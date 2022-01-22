#include <doge/extensions/physics/physics.hpp>

#include <doge/core/Engine.hpp>
#include <doge/components.hpp>
#include <doge/extensions/physics.hpp>
#include <algorithm>

namespace doge
{
    std::unique_ptr<b2World> physics::world;
    std::unordered_map<EntityID, b2Body*> physics::bodies;
    std::unordered_map<EntityID, physics::BodyInit> physics::body_inits;
    std::map<physics::FixtureKey, b2Fixture*> physics::fixtures;
    Vec2f physics::gravity = Vec2f(0.f, 9.8f);

    void physics::Enable(Engine& engine)
    {
        GameLoopFunctions glf;
        glf.start = Start;
        glf.update = Update;
        glf.fixed_update = FixedUpdate;
        glf.finish = Finish;
        
        engine.scenes.extensions.emplace("doge_physics", glf);
        world = std::make_unique<b2World>(ToB2Vec2(gravity));
    }

    void physics::Disable(Engine& engine)
    {
        world.release();
        engine.scenes.extensions.erase("doge_physics");
    }

    void physics::SetGravity(const Vec2f& gravity)
    {
        if (world)
            world->SetGravity(ToB2Vec2(gravity));
        physics::gravity = gravity;
    }

    const Vec2f& physics::GetGravity()
    {
        return gravity;
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

    bool physics::HasBody(EntityID entity_id)
    {
        return bodies.find(entity_id) != bodies.end();
    }

    bool physics::HasCompoundCollider(EntityID entity_id)
    {
        return std::find_if(fixtures.begin(), fixtures.end(),
        [&](const std::pair<FixtureKey, b2Fixture*>& p)
        { return std::get<0>(p.first) == entity_id; })
        != fixtures.end();
    }

    physics::Collider physics::GetCollider(EntityID entity_id)
    {
        return Collider(GetBody(entity_id).b2_body->GetFixtureList());
    }

    physics::Collider physics::GetCollider(EntityID entity_id, Collider::Type type, std::size_t index)
    {
        return Collider(fixtures.at(FixtureKey(entity_id, type, index)));
    }
    
    Vec2f physics::FromB2Vec2(const b2Vec2& v)
    {
        return Vec2f(v.x, v.y);
    }

    b2Vec2 physics::ToB2Vec2(const Vec2f& v)
    {
        return b2Vec2(v.x, v.y);
    }

    b2BodyType physics::ToB2BodyType(RigidBody::Type type)
    {
        switch (type)
        {
            case RigidBody::Type::Static:       return b2BodyType::b2_staticBody;
            case RigidBody::Type::Kinematic:    return b2BodyType::b2_kinematicBody;
            case RigidBody::Type::Dynamic:      return b2BodyType::b2_dynamicBody;
            default: throw std::invalid_argument("Invalid b2 body type");
        }
    }

    RigidBody::Type physics::FromB2BodyType(b2BodyType type)
    {
        switch (type)
        {
            case b2BodyType::b2_staticBody:     return RigidBody::Type::Static;
            case b2BodyType::b2_kinematicBody:  return RigidBody::Type::Kinematic;
            case b2BodyType::b2_dynamicBody:    return RigidBody::Type::Dynamic;
            default: throw std::invalid_argument("Invalid b2 body type");
        }
    }

    void physics::Start(Engine& engine)
    {
    }

    void physics::Update(Engine& engine, DeltaTime dt)
    {
        // helper functions
        auto CreateBody = [&](const Entity& entity, const RigidBody& rgbd) -> b2Body*
        {
            b2BodyDef body_def;
            body_def.type = ToB2BodyType(rgbd.type);

            auto pos = entity.GetIfHasComponentElseDefault<Position>().position;
            body_def.position = ToB2Vec2(pos);

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

        auto EmplaceBody = [&](EntityID eid, const Component<RigidBody>& rgbd, b2Body* body)
        {
            auto [body_itr, success] = bodies.emplace(eid, body);

            rgbd.OnRemoval([&, eid]()
            {
                if (bodies.find(eid) == bodies.end())
                    return;
                
                if (world)
                    world->DestroyBody(bodies.at(eid));
                
                bodies.erase(eid);
            });

            return body_itr;
        };

        auto EmplaceFixture = [&]<typename TComp>(const FixtureKey& key, b2Fixture* fixture, EntityID rgbd_entity, Component<TComp>& coll_comp)
        {
            auto [itr, success] = fixtures.emplace(key, fixture);
            if (!success)
                throw std::invalid_argument("More than one collider component found in entity");

            coll_comp.OnRemoval([&, rgbd_entity, val_key = key]()
            {
                if (fixtures.find(val_key) == fixtures.end())
                    return;
                
                auto body_itr = bodies.find(rgbd_entity);
                if (body_itr != bodies.end())
                    body_itr->second->DestroyFixture(fixtures.at(val_key));

                fixtures.erase(val_key);
            });

            return itr;
        };

        auto AddFixture = [&]<typename TComp>(b2Body* body, const TComp& coll_comp, const b2Shape* shape)
        {
            b2FixtureDef fixture_def;
            fixture_def.shape = shape;
            fixture_def.density = coll_comp.density;
            fixture_def.friction = coll_comp.friction;
            fixture_def.restitution = coll_comp.restitution;
            fixture_def.restitutionThreshold = coll_comp.restitution_threshold;
            fixture_def.isSensor = coll_comp.is_trigger;

            b2Fixture* fixture = body->CreateFixture(&fixture_def);
            return fixture;
        };

        auto SyncFixture = []<typename TComp>(b2Fixture* fixture, const TComp& coll)
        {
            fixture->SetDensity(coll.density);
            fixture->SetFriction(coll.friction);
            fixture->SetRestitution(coll.restitution);
            fixture->SetRestitutionThreshold(coll.restitution_threshold);
            fixture->SetSensor(coll.is_trigger);
        };

        // enable disable
        for (auto [entity, entity_info, rgbd] : engine.Select<EntityInfo, RigidBody>(true).EntitiesAndComponents())
        {
            auto body_itr = bodies.find(entity);
            if (body_itr != bodies.end())
            {
                b2Body* body = body_itr->second;
                bool is_enabled = entity_info.enabled && rgbd.enabled;

                if (is_enabled != body->IsEnabled())
                    body->SetEnabled(is_enabled);

                if (!is_enabled)
                    continue;
                
                body->SetAwake(rgbd.awake);
            }
        }

        // rigidbody
        for (auto [entity, rgbd] : engine.Select<RigidBody>().EntitiesAndComponents())
        {
            auto body_itr = bodies.find(entity);
            if (body_itr == bodies.end())
            {
                body_itr = EmplaceBody(entity, rgbd, CreateBody(entity, rgbd));
            }
            
            b2Body* body = body_itr->second;

            auto pos = body->GetPosition();
            if (entity.HasComponent<Position>())
                pos = ToB2Vec2(entity.GetComponent<Position>().position);
            
            auto angle = body->GetAngle();
            if (entity.HasComponent<Rotation>())
                angle = entity.GetComponent<Rotation>().rotation;
            
            body->SetTransform(pos, angle);

            if (ToB2BodyType(rgbd.type) != body->GetType())
                body->SetType(ToB2BodyType(rgbd.type));
            body->SetBullet(rgbd.continuous);
        }

        // convex collider
        auto SyncConvex = [](b2PolygonShape& convex, const Entity& entity, const ConvexCollider& coll)
        {
            auto scale = global::GetScale(entity);
            std::vector<b2Vec2> vertices;
            std::transform(coll.points.begin(), coll.points.end(), std::back_inserter(vertices), 
            [&](const Vec2f& v) { return ToB2Vec2((v - coll.origin) * scale); });
            convex.Set(vertices.data(), vertices.size());
        };

        auto UpdateConvex = [&]<typename TComp>(Entity& entity, Component<TComp>& coll_comp, ConvexCollider& coll, const FixtureKey& key)
        {
            auto body_itr = bodies.find(coll.rigidbody_entity);

            if (body_itr == bodies.end())
                throw std::invalid_argument("Invalid ConvexCollider::rigidbody_entity, can't find the attached rigidbody");

            auto* body = body_itr->second;

            auto fixture_itr = fixtures.find(key);
            if (fixture_itr == fixtures.end())
            {
                b2PolygonShape convex;
                SyncConvex(convex, entity, coll);
                fixture_itr = EmplaceFixture(key, AddFixture(body, coll, &convex), coll.rigidbody_entity, coll_comp);
            }
            
            if (coll.apply_changes)
            {
                auto* shape = static_cast<b2PolygonShape*>(fixture_itr->second->GetShape());

                if (shape->m_count != coll.points.size())
                {
                    body->DestroyFixture(fixture_itr->second);
                    b2PolygonShape convex;
                    SyncConvex(convex, entity, coll);
                    fixture_itr->second = AddFixture(body, coll, &convex);
                }
                else
                {
                    SyncFixture(fixture_itr->second, coll);
                    SyncConvex(*shape, entity, coll);
                }
                coll.apply_changes = false;
            }
        };

        for (auto [entity, coll] : engine.Select<ConvexCollider>().EntitiesAndComponents())
        {
            UpdateConvex(entity, coll, coll, FixtureKey(entity, Collider::Type::Convex, 0));
        }

        // circle collider
        auto SyncCircle = [](b2CircleShape& circle, const Entity& entity, const CircleCollider& coll)
        {
            auto scale = global::GetScale(entity);
            circle.m_radius = coll.radius * scale.x;
            circle.m_p = ToB2Vec2(-coll.origin * scale);
        };

        auto UpdateCircle = [&]<typename TComp>(Entity& entity, Component<TComp>& coll_comp, CircleCollider& coll, const FixtureKey& key)
        {
            auto body_itr = bodies.find(coll.rigidbody_entity);

            if (body_itr == bodies.end())
                throw std::invalid_argument("Invalid CircleCollider::rigidbody_entity, can't find the attached rigidbody");

            auto* body = body_itr->second;

            auto fixture_itr = fixtures.find(key);
            if (fixture_itr == fixtures.end())
            {
                b2CircleShape circle;
                SyncCircle(circle, entity, coll);
                fixture_itr = EmplaceFixture(key, AddFixture(body, coll, &circle), coll.rigidbody_entity, coll_comp);
            }
            
            if (coll.apply_changes)
            {
                auto* shape = static_cast<b2CircleShape*>(fixture_itr->second->GetShape());
                SyncFixture(fixture_itr->second, coll);
                SyncCircle(*shape, entity, coll);
                coll.apply_changes = false;
            }
        };

        for (auto [entity, coll] : engine.Select<CircleCollider>().EntitiesAndComponents())
        {
            UpdateCircle(entity, coll, coll, FixtureKey(entity, Collider::Type::Circle, 0));
        }

        // rectangle collider
        auto SyncRectangle = [](b2PolygonShape& rect, const Entity& entity, const RectangleCollider& coll)
        {
            auto scale = global::GetScale(entity);
            rect.SetAsBox(
                coll.size.x * scale.x / 2.f, coll.size.y * scale.y / 2.f, 
                ToB2Vec2(-coll.origin), 
                0
            );
        };

        auto UpdateRectangle = [&]<typename TComp>(Entity& entity, Component<TComp>& coll_comp, RectangleCollider& coll, const FixtureKey& key)
        {
            auto body_itr = bodies.find(coll.rigidbody_entity);

            if (body_itr == bodies.end())
                throw std::invalid_argument("Invalid RectangleCollider::rigidbody_entity, can't find the attached rigidbody");

            auto* body = body_itr->second;

            auto fixture_itr = fixtures.find(key);
            if (fixture_itr == fixtures.end())
            {
                b2PolygonShape rect;
                SyncRectangle(rect, entity, coll);
                fixture_itr = EmplaceFixture(key, AddFixture(body, coll, &rect), coll.rigidbody_entity, coll_comp);
            }
            
            if (coll.apply_changes)
            {
                auto* shape = static_cast<b2PolygonShape*>(fixture_itr->second->GetShape());
                SyncFixture(fixture_itr->second, coll);
                SyncRectangle(*shape, entity, coll);
                coll.apply_changes = false;
            }
        };

        for (auto [entity, coll] : engine.Select<RectangleCollider>().EntitiesAndComponents())
        {
            UpdateRectangle(entity, coll, coll, FixtureKey(entity, Collider::Type::Rectangle, 0));
        }

        // edge collider
        auto SyncEdge = [](b2ChainShape& chain, const Entity& entity, const EdgeCollider& coll)
        {
            auto scale = global::GetScale(entity);
            std::vector<b2Vec2> vertices;
            std::transform(coll.points.begin(), coll.points.end(), std::back_inserter(vertices), 
            [&](const Vec2f& v) { return ToB2Vec2((v - coll.origin) * scale); });
            if (coll.is_loop)
                chain.CreateLoop(vertices.data(), vertices.size());
            else
                chain.CreateChain(vertices.data(), vertices.size(), vertices.front(), vertices.back());
        };

        auto UpdateEdge = [&]<typename TComp>(Entity& entity, Component<TComp>& coll_comp, EdgeCollider& coll, const FixtureKey& key)
        {
            auto body_itr = bodies.find(coll.rigidbody_entity);

            if (body_itr == bodies.end())
                throw std::invalid_argument("Invalid EdgeCollider::rigidbody_entity, can't find the attached rigidbody");

            auto* body = body_itr->second;

            auto fixture_itr = fixtures.find(key);
            if (fixture_itr == fixtures.end())
            {
                b2ChainShape edge;
                SyncEdge(edge, entity, coll);
                fixture_itr = EmplaceFixture(key, AddFixture(body, coll, &edge), coll.rigidbody_entity, coll_comp);
            }

            if (coll.apply_changes)
            {
                auto* shape = static_cast<b2ChainShape*>(fixture_itr->second->GetShape());
                if (shape->m_count != coll.points.size())
                {
                    body->DestroyFixture(fixture_itr->second);
                    b2ChainShape chain;
                    SyncEdge(chain, entity, coll);
                    AddFixture(body, coll, &chain);
                }
                else
                {
                    SyncFixture(fixture_itr->second, coll);
                    SyncEdge(*shape, entity, coll);
                }
                coll.apply_changes = false;
            }
        };

        for (auto [entity, coll] : engine.Select<EdgeCollider>().EntitiesAndComponents())
        {
            UpdateEdge(entity, coll, coll, FixtureKey(entity, Collider::Type::Edge, 0));
        }

        // compound collider
        for (auto [entity, coll] : engine.Select<CompoundCollider>().EntitiesAndComponents())
        {
            if (coll.apply_changes)
            {
                std::erase_if(fixtures,
                [&](const std::pair<FixtureKey, b2Fixture*>& fixture)
                { return std::get<0>(fixture.first) == entity.id; });
            }

            for (std::size_t i = 0; i < coll.convex_colliders.size(); ++i)
            {
                UpdateConvex(entity, coll, coll.convex_colliders.at(i), FixtureKey(entity, Collider::Type::Convex, i));
            }

            for (std::size_t i = 0; i < coll.circle_colliders.size(); ++i)
            {
                UpdateCircle(entity, coll, coll.circle_colliders.at(i), FixtureKey(entity, Collider::Type::Circle, i));
            }

            for (std::size_t i = 0; i < coll.rectangle_colliders.size(); ++i)
            {
                UpdateRectangle(entity, coll, coll.rectangle_colliders.at(i), FixtureKey(entity, Collider::Type::Rectangle, i));
            }
        }

        // for (auto [entity, rgbd, coll] : engine.Select<RigidBody, ConvexCollider>().EntitiesAndComponents())
        // {
        //     auto body_itr = bodies.find(entity.id);
        //     if (body_itr == bodies.end())
        //     {
        //         b2PolygonShape convex;
        //         SyncConvex(convex, entity, coll);
        //         AddFixture(EmplaceBody(entity, rgbd, CreateBody(entity, rgbd))->second, coll, &convex);
        //     }
        //     else if (coll.apply_changes)
        //     {
        //         auto* body = body_itr->second;
        //         auto* fixture = body->GetFixtureList();
        //         auto* shape = static_cast<b2PolygonShape*>(fixture->GetShape());

        //         if (shape->m_count != coll.points.size())
        //         {
        //             body->DestroyFixture(fixture);
        //             b2PolygonShape convex;
        //             SyncConvex(convex, entity, coll);
        //             AddFixture(body, coll, &convex);
        //         }
        //         else
        //         {
        //             SyncFixture(fixture, coll);
        //             SyncConvex(*shape, entity, coll);
        //         }

        //         coll.apply_changes = false;
        //     }
        // }

        // // circle collider
        // auto SyncCircle = [](b2CircleShape& circle, const Entity& entity, const CircleCollider& coll)
        // {
        //     auto scale = global::GetScale(entity);
        //     circle.m_radius = coll.radius * scale.x;
        //     circle.m_p = ToB2Vec2(-coll.origin * scale);
        // };

        // for (auto [entity, rgbd, coll] : engine.Select<RigidBody, CircleCollider>().EntitiesAndComponents())
        // {
        //     auto body_itr = bodies.find(entity.id);
        //     if (body_itr == bodies.end())
        //     {
        //         b2CircleShape circle;
        //         SyncCircle(circle, entity, coll);

        //         AddFixture(EmplaceBody(entity, rgbd, CreateBody(entity, rgbd))->second, coll, &circle);
        //     }
        //     else if (coll.apply_changes)
        //     {
        //         auto* body = body_itr->second;
        //         auto* fixture = body->GetFixtureList();
        //         auto* shape = static_cast<b2CircleShape*>(fixture->GetShape());

        //         SyncFixture(fixture, coll);
        //         SyncCircle(*shape, entity, coll);

        //         coll.apply_changes = false;
        //     }
        // }

        // // rectangle collider
        // auto SyncRectangle = [](b2PolygonShape& rect, const Entity& entity, const RectangleCollider& coll)
        // {
        //     auto scale = global::GetScale(entity);
        //     rect.SetAsBox(
        //         coll.size.x * scale.x / 2.f, coll.size.y * scale.y / 2.f, 
        //         ToB2Vec2(-coll.origin), 
        //         0
        //     );
        // };

        // for (auto [entity, rgbd, coll] : engine.Select<RigidBody, RectangleCollider>().EntitiesAndComponents())
        // {
        //     auto body_itr = bodies.find(entity.id);
        //     if (body_itr == bodies.end())
        //     {
        //         b2PolygonShape rect;
        //         SyncRectangle(rect, entity, coll);

        //         AddFixture(EmplaceBody(entity, rgbd, CreateBody(entity, rgbd))->second, coll, &rect);
        //     }
        //     else if (coll.apply_changes)
        //     {
        //         auto* body = body_itr->second;
        //         auto* fixture = body->GetFixtureList();
        //         auto* shape = static_cast<b2PolygonShape*>(fixture->GetShape());

        //         SyncFixture(fixture, coll);
        //         SyncRectangle(*shape, entity, coll);

        //         coll.apply_changes = false;
        //     }
        // }

        // // edge collider
        // auto SyncEdge = [](b2ChainShape& chain, const Entity& entity, const EdgeCollider& coll)
        // {
        //     auto scale = global::GetScale(entity);
        //     std::vector<b2Vec2> vertices;
        //     std::transform(coll.points.begin(), coll.points.end(), std::back_inserter(vertices), 
        //     [&](const Vec2f& v) { return ToB2Vec2((v - coll.origin) * scale); });
        //     if (coll.is_loop)
        //         chain.CreateLoop(vertices.data(), vertices.size());
        //     else
        //         chain.CreateChain(vertices.data(), vertices.size(), vertices.front(), vertices.back());
        // };

        // for (auto [entity, rgbd, coll] : engine.Select<RigidBody, EdgeCollider>().EntitiesAndComponents())
        // {
        //     auto body_itr = bodies.find(entity.id);
        //     if (body_itr == bodies.end())
        //     {
        //         b2ChainShape chain;
        //         SyncEdge(chain, entity, coll);

        //         AddFixture(EmplaceBody(entity, rgbd, CreateBody(entity, rgbd))->second, coll, &chain);
        //     }
        //     else if (coll.apply_changes)
        //     {
        //         auto* body = body_itr->second;
        //         auto* fixture = body->GetFixtureList();
        //         auto* shape = static_cast<b2ChainShape*>(fixture->GetShape());

        //         if (shape->m_count != coll.points.size())
        //         {
        //             body->DestroyFixture(fixture);
        //             b2ChainShape chain;
        //             SyncEdge(chain, entity, coll);
        //             AddFixture(body, coll, &chain);
        //         }
        //         else
        //         {
        //             SyncFixture(fixture, coll);
        //             SyncEdge(*shape, entity, coll);
        //         }

        //         coll.apply_changes = false;
        //     }
        // }

        // // compound collider
        // for (auto [entity, rgbd, coll] : engine.Select<RigidBody, CompoundCollider>().EntitiesAndComponents())
        // {
        //     auto body_itr = bodies.find(entity.id);
        //     if (body_itr == bodies.end() || coll.apply_changes)
        //     {
        //         auto scale = global::GetScale(entity);

        //         if (body_itr == bodies.end())
        //         {
        //             body_itr = EmplaceBody(entity, rgbd, CreateBody(entity, rgbd));
        //             compound_fixtures.emplace(entity, std::array<std::vector<b2Fixture*>, Collider::Type::Count>
        //             { 
        //                 std::vector<b2Fixture*>(coll.convex_colliders.size()), 
        //                 std::vector<b2Fixture*>(coll.circle_colliders.size()), 
        //                 std::vector<b2Fixture*>(coll.rectangle_colliders.size()) 
        //             });
        //         }

        //         auto& fixtures = compound_fixtures.at(entity);
        //         auto* body = body_itr->second;

        //         // clear fixture
        //         if (coll.apply_changes)
        //         {
        //             auto* fixture = body->GetFixtureList();
        //             b2Fixture* next = nullptr;
        //             while (fixture != nullptr)
        //             {
        //                 next = fixture->GetNext();
        //                 body->DestroyFixture(fixture);
        //                 fixture = next;
        //             }

        //             fixtures = std::array<std::vector<b2Fixture*>, Collider::Type::Count>
        //             { 
        //                 std::vector<b2Fixture*>(coll.convex_colliders.size()), 
        //                 std::vector<b2Fixture*>(coll.circle_colliders.size()), 
        //                 std::vector<b2Fixture*>(coll.rectangle_colliders.size()) 
        //             };

        //             coll.apply_changes = false;
        //         }

        //         // convex sub collider
        //         for (auto i = 0; i < coll.convex_colliders.size(); ++i)
        //         {
        //             auto& convex_coll = coll.convex_colliders.at(i);

        //             b2PolygonShape convex;
        //             SyncConvex(convex, entity, convex_coll);

        //             fixtures.at(Collider::Type::Convex).at(i) = AddFixture(body, convex_coll, &convex).second;

        //             convex_coll.apply_changes = false;
        //         }

        //         // circle sub collider
        //         for (auto i = 0; i < coll.circle_colliders.size(); ++i)
        //         {
        //             auto& circle_coll = coll.circle_colliders.at(i);

        //             b2CircleShape circle;
        //             SyncCircle(circle, entity, circle_coll);

        //             fixtures.at(Collider::Type::Circle).at(i) = AddFixture(body, circle_coll, &circle).second;

        //             circle_coll.apply_changes = false;
        //         }

        //         // rectangle sub collider
        //         for (auto i = 0; i < coll.rectangle_colliders.size(); ++i)
        //         {
        //             auto& rectangle_coll = coll.rectangle_colliders.at(i);

        //             b2PolygonShape rect;
        //             rect.SetAsBox(
        //                 rectangle_coll.size.x * scale.x / 2.f, rectangle_coll.size.y * scale.y / 2.f, 
        //                 ToB2Vec2(rectangle_coll.origin), 
        //                 0
        //             );

        //             fixtures.at(Collider::Type::Rectangle).at(i) = AddFixture(body, rectangle_coll, &rect).second;

        //             rectangle_coll.apply_changes = false;
        //         }
        //     }
        //     else
        //     {
        //         auto* body = body_itr->second;
                
        //         // convex sub collider
        //         for (auto i = 0; i < coll.convex_colliders.size(); ++i)
        //         {
        //             auto& convex_coll = coll.convex_colliders.at(i);

        //             if (!convex_coll.apply_changes)
        //                 continue;

        //             auto* body = body_itr->second;
        //             auto*& fixture = compound_fixtures.at(entity).at(Collider::Type::Convex).at(i);
        //             auto* shape = static_cast<b2PolygonShape*>(fixture->GetShape());

        //             if (shape->m_count != convex_coll.points.size())
        //             {
        //                 body->DestroyFixture(fixture);
        //                 b2PolygonShape convex;
        //                 SyncConvex(convex, entity, convex_coll);
        //                 fixture = AddFixture(body, convex_coll, &convex).second;
        //             }
        //             else
        //             {
        //                 SyncFixture(fixture, convex_coll);
        //                 SyncConvex(*shape, entity, convex_coll);
        //             }

        //             coll.apply_changes = false;
        //         }

        //         // circle sub collider
        //         for (auto i = 0; i < coll.circle_colliders.size(); ++i)
        //         {
        //             auto& circle_coll = coll.circle_colliders.at(i);

        //             if (!circle_coll.apply_changes)
        //                 continue;

        //             auto* body = body_itr->second;
        //             auto*& fixture = compound_fixtures.at(entity).at(Collider::Type::Circle).at(i);
        //             auto* shape = static_cast<b2CircleShape*>(fixture->GetShape());

        //             SyncFixture(fixture, circle_coll);
        //             SyncCircle(*shape, entity, circle_coll);

        //             circle_coll.apply_changes = false;
        //         }

        //         // rectangle sub collider
        //         for (auto i = 0; i < coll.rectangle_colliders.size(); ++i)
        //         {
        //             auto& rectangle_coll = coll.rectangle_colliders.at(i);

        //             if (!rectangle_coll.apply_changes)
        //                 continue;

        //             auto* body = body_itr->second;
        //             auto*& fixture = compound_fixtures.at(entity).at(Collider::Type::Rectangle).at(i);
        //             auto* shape = static_cast<b2PolygonShape*>(fixture->GetShape());

        //             SyncFixture(fixture, rectangle_coll);
        //             SyncRectangle(*shape, entity, rectangle_coll);

        //             rectangle_coll.apply_changes = false;
        //         }
        //     }
        // }

        // // rigid body only
        // for (auto [entity, rgbd] : engine.Select<RigidBody>().EntitiesAndComponents())
        // {
        //     auto body_itr = bodies.find(rgbd.entity_id);
        //     if (body_itr == bodies.end())
        //     {
        //         b2BodyDef body_def;
        //         body_def.type = ToB2BodyType(rgbd.type);
        //         auto pos = entity.GetIfHasComponentElseDefault<Position>().position;
        //         body_def.position = ToB2Vec2(pos);
        //         body_def.angle = entity.GetIfHasComponentElseDefault<Rotation>().rotation;

        //         EmplaceBody(entity, rgbd, world->CreateBody(&body_def));
        //     }
        //     else
        //     {
        //         b2Body* body = body_itr->second;

        //         auto pos = body->GetPosition();
        //         if (entity.HasComponent<Position>())
        //             pos = ToB2Vec2(entity.GetComponent<Position>().position);

        //         auto angle = body->GetAngle();
        //         if (entity.HasComponent<Rotation>())
        //             angle = entity.GetComponent<Rotation>().rotation;

        //         body->SetTransform(pos, angle);

        //         if (ToB2BodyType(rgbd.type) != body->GetType())
        //             body->SetType(ToB2BodyType(rgbd.type));

        //         body->SetBullet(rgbd.continuous);
        //     }
        // }

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
                    entity.GetComponent<Position>().position = FromB2Vec2(body->GetPosition());

                if (entity.HasComponent<Rotation>())
                    entity.GetComponent<Rotation>().rotation = body->GetAngle();
            }
        }
    }

    void physics::Finish(Engine& engine)
    {
        bodies.clear();
    }
}