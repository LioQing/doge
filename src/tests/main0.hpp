#include <iostream>

#include <doge/doge.hpp>
#include <doge/extensions/physics.hpp>
#include <box2d/box2d.h>
#include <ctime>
#include <algorithm>
#include <numbers>

namespace main0
{
    std::unique_ptr<doge::Physics> phy = nullptr;

    doge::Entity AddGreenRect(doge::Engine& e)
    {
        auto my_shape = e.AddEntity(false);

        auto& rgbd = my_shape.AddComponent(doge::RigidBody
        {
            .type = doge::RigidBody::Type::Dynamic, 
            //.continuous = true,
        });

        my_shape.AddComponent(doge::ConvexCollider
        {
            .rigid_body_entity = my_shape,
            .points =
            {
                { 10.f, 10.f }, { -10.f, 10.f }, { -20.f, -10.f }, { 20.f, -10.f }
            },
            .density = 1.f,
            .friction = .3f,
            .restitution = .5f,
        });

        auto instant_dest = e.AddEntity();
        instant_dest.AddComponent<doge::Scale>(0.01, 0.01);
        instant_dest.AddComponent<doge::Position>();
        instant_dest.AddComponent(doge::CircleCollider
        {
            .rigid_body_entity = my_shape,
            .radius = 50.f,
        });
        instant_dest.AddComponent(doge::CircleShape
        {
            .radius = 50.f,
            .origin = doge::Vec2f(50.f, 50.f),
            .color = 0xFFFFFF01,
        });
        e.DestroyEntity(instant_dest);

        my_shape.AddComponent<doge::Position>();
        my_shape.AddComponent<doge::Rotation>(std::fmod(rand(), std::numbers::pi * 2));

        doge::Physics::BodyInit init;
        init.velocity = doge::Vec2f(std::fmod(rand(), 10) - 5, std::fmod(rand(), 10) - 5).Normalized() * 200.f * 0.01;
        init.angular_velocity = 20.f;
        phy->SetBodyInit(my_shape, init);

        my_shape.AddComponent(doge::ConvexShape
        {
            .points =
            {
                { 10.f, 10.f }, { -10.f, 10.f }, { -20.f, -10.f }, { 20.f, -10.f }
            },
            .color = doge::Color(0x00FF0088),
        });

        my_shape.AddComponent<doge::Scale>(0.02, 0.02);

        return my_shape;
    }

    void AddBlocks(doge::Engine& e)
    {
        for (auto i = 0; i < 10; ++i)
        {
            auto entity = AddGreenRect(e);
            entity.GetComponent<doge::Position>().position = doge::Vec2f(50.f * i, 0.f) * 0.01;
        }

        for (auto i = 1; i < 11; ++i)
        {
            auto my_shape = e.AddEntity();

            my_shape.AddComponent(doge::RigidBody
            {
                .type = doge::RigidBody::Type::Dynamic, 
            });

            my_shape.AddComponent(doge::CompoundCollider::Create
            (
                doge::CircleCollider
                {
                    .rigid_body_entity = my_shape,
                    .radius = 10.f,
                    .origin = { 15, 0 },
                    .density = 1.f,
                    .friction = .3f,
                    .restitution = .5f,
                },
                doge::RectangleCollider
                {
                    .rigid_body_entity = my_shape,
                    .size = { 30, 8 },
                    .origin = { 0, 0 },
                    .density = 1.f,
                    .friction = .3f,
                    .restitution = .5f,
                }
            ));

            my_shape.AddComponent<doge::Position>(-i * 0.5, 0);
            my_shape.AddComponent<doge::Rotation>(std::fmod(rand(), std::numbers::pi * 2));
            my_shape.AddComponent<doge::Scale>(0.01, 0.01);

            doge::Physics::BodyInit init;
            init.velocity = doge::Vec2f(std::fmod(rand(), 10) - 5, std::fmod(rand(), 10) - 5).Normalized() * 50.f * 0.01;
            init.angular_velocity = 20.f;
            phy->SetBodyInit(my_shape, init);

            // auto my_comp = my_shape.AddComponent(doge::CompoundSprite::Create
            // (
            my_shape.AddComponent(
                doge::CircleShape
                {
                    .radius = 10.f,
                    .origin = doge::Vec2f(25.f, 10.f),
                    .color = doge::Color(0x0000FF88),
                });
            my_shape.AddComponent(
                doge::RectangleShape
                {
                    .size = { 30, 8 },
                    .origin = { 15, 4 },
                    .color = doge::Color(0x0000FF88),
                });
            //));
        }

        auto my_sprite = e.AddEntity();
        my_sprite.AddComponent(doge::Sprite
        {
            .texture_rectangle = { 0, 0, 100, 100 },
            .origin = { 50, 50 },
        });
        my_sprite.AddComponent(doge::RigidBody{ .type = doge::RigidBody::Type::Dynamic });
        my_sprite.AddComponent<doge::Position>(0, -2);
        my_sprite.AddComponent<doge::Rotation>();
        my_sprite.AddComponent(doge::RectangleCollider
        {
            .rigid_body_entity = my_sprite,
            .size = { 100, 100 },
        });
        my_sprite.AddComponent<doge::Scale>(0.01, 0.01);

        auto aabb = e.AddEntity();
        aabb.SetParent(my_sprite);
        aabb.AddComponent(doge::Tag::Create(std::to_string(my_sprite.id)));
        aabb.AddComponent(doge::RectangleShape
        {
            .color = doge::Color::Transparent,
            .outline_color = doge::Color::White,
            .outline_thickness = 0.01f,
        });
        aabb.AddComponent<doge::Position>();
        aabb.AddComponent<doge::Scale>();
        aabb.AddComponent<doge::Rotation>();
    }

    int count = 0;
    doge::Entity cam;
    doge::Vec2f mouse_down;
    doge::Vec2f mouse_down_entity_pos;
    float mouse_down_entity_rot;
    int mouse_down_id = -1;
    doge::Entity line;

    void Start(doge::Engine& e)
    {
        phy = std::make_unique<doge::Physics>(e);
        phy->SetGravity(doge::Vec2f(0, 1.96f));

        count = 0;

        cam = e.AddCamera();
        cam.AddComponent<doge::Position>();
        cam.AddComponent<doge::Rotation>();
        cam.AddComponent<doge::Scale>(0.01, 0.01);

        auto ground = e.AddEntity();
        ground.AddComponent<doge::RigidBody>(doge::RigidBody::Type::Static);
        ground.AddComponent(doge::ConvexCollider
        {
            .rigid_body_entity = ground,
            .points = // shape: /\.
            { 
                doge::Vec2f(-static_cast<float>(e.window.settings.size.x) / 2.f + 100, 0),
                doge::Vec2f(0, -100), 
                doge::Vec2f(e.window.settings.size.x / 2.f - 100, 0), 
            },
        });
        ground.AddComponent<doge::Position>(0.f, (e.window.settings.size.y / 2.f - 100.f) * 0.01);

        ground.AddComponent(doge::ConvexShape
        {
            .points = // shape: /\.
            { 
                doge::Vec2f(-static_cast<float>(e.window.settings.size.x) / 2.f + 100, 0),
                doge::Vec2f(0, -100), 
                doge::Vec2f(e.window.settings.size.x / 2.f - 100, 0), 
            },
            .color = doge::Color::Red,
        });

        ground.AddComponent<doge::Scale>(0.01, 0.01);

        auto my_custom_shape = e.AddEntity();
        my_custom_shape.AddComponent(doge::PolygonShape
        {
            .type = doge::PolygonShape::Type::Triangles,
            .vertices = 
            {
                doge::PolygonShape::Vertex({ 0, 0 }, doge::Color::Red),
                doge::PolygonShape::Vertex({ 100, 100 }, doge::Color::Green),
                doge::PolygonShape::Vertex({ 100, 0 }, doge::Color::Blue),
            },
            .origin = { 50, 50 },
            .texture_id = "missing_texture",
        });
        my_custom_shape.AddComponent(doge::RigidBody(doge::RigidBody::Kinematic));
        my_custom_shape.AddComponent(doge::ConvexCollider
        {
            .rigid_body_entity = my_custom_shape,
            .points = 
            {
                doge::Vec2f(0, 0),
                doge::Vec2f(100, 100),
                doge::Vec2f(100, 0),
            },
            .origin = { 50, 50 },
        });
        my_custom_shape.AddComponent<doge::Rotation>();
        my_custom_shape.AddComponent<doge::Scale>(0.01, 0.01);

        phy->SetBodyInit(my_custom_shape.id, doge::Physics::BodyInit{ .angular_velocity = 5.f });

        AddBlocks(e);

        e.events.on_key_pressed += [&e](const doge::event::Key& key)
        {
            if (key.key == doge::event::Key::Code::Space)
            {
                auto scrnshot = e.window.TakeScreenshot();

                scrnshot.ToFile("screenshot.png");
            }
        };

        e.events.on_mouse_button_pressed += [&](const doge::event::MouseButton& button)
        {
            if (button.button == doge::event::MouseButton::Button::Left)
            {
                for (auto entity : e.Select<doge::ConvexCollider>().Entities())
                {
                    mouse_down = e.window.MapPixelToCoords(button.position, cam.GetComponent<doge::Camera>());
                    if (!phy->GetCollider(entity).TestPoint(mouse_down))
                        continue;

                    line.GetComponent<doge::Position>().position = mouse_down;
                    mouse_down_entity_pos = entity.GetIfHasComponentElseDefault<doge::Position>().position;
                    mouse_down_entity_rot = entity.GetIfHasComponentElseDefault<doge::Rotation>().rotation;
                    mouse_down_id = entity.id;

                    std::cout << mouse_down_id << mouse_down_entity_pos << std::endl;

                    return;
                }
            }
        };

        e.events.on_mouse_button_released += [&](const doge::event::MouseButton& button)
        {
            if (button.button == doge::event::MouseButton::Button::Left)
            {
                if (mouse_down_id == -1) return;

                std::cout << line << " " << mouse_down_id << std::endl;

                if (!e.HasEntity(mouse_down_id) || !phy->HasBody(mouse_down_id))
                    return;
                
                phy->GetBody(mouse_down_id).ApplyImpulse((mouse_down - e.window.MapPixelToCoords(button.position, cam.GetComponent<doge::Camera>())), mouse_down);
                mouse_down_id = -1;
            }
        };

        line = e.AddEntity();
        line.AddComponent(doge::Tag::Create("line"));
        line.AddComponent<doge::Position>();
        line.AddComponent(doge::PolygonShape
        { 
            .type = doge::PolygonShape::Lines, 
            .vertices = 
            { 
                doge::PolygonShape::Vertex(doge::Vec2f(0, 0), doge::Color::White), 
                doge::PolygonShape::Vertex(doge::Vec2f(0, 0), doge::Color::White), 
            } 
        });
    }

    void Update(doge::Engine& e, doge::DeltaTime dt)
    {
        if (++count > 100)
        {
            AddBlocks(e);
            count = 0;
        }

        for (auto [entity, rgbd, scale, position, convex, coll, rot] : e.Select<doge::RigidBody, doge::Scale, doge::Position, doge::ConvexShape, doge::ConvexCollider, doge::Rotation>().EntitiesAndComponents())
        {
            if (entity == mouse_down_id)
            {
                auto body = phy->GetBody(entity);

                if (position.position != mouse_down_entity_pos)
                {
                    auto dir = (mouse_down_entity_pos - position.position);
                    body.ApplyForce(doge::Vec2f(std::pow(10, dir.x), std::pow(10, dir.y)) * dir * 100.f);
                }

                if (rot.rotation != mouse_down_entity_rot)
                    body.ApplyTorque((mouse_down_entity_rot - rot.rotation) * 1.f);
            }
            else if (doge::global::GetAABB(convex).top > e.window.settings.size.y / 2.f * 0.01)
                e.DestroyEntity(entity);
        }

        // for (auto [entity, rgbd, position, shape, coll] : e.Select<doge::RigidBody, doge::Position, doge::CompoundSprite, doge::CompoundCollider>().EntitiesAndComponents())
        // {
        //     auto aabb = doge::global::GetAABB(shape.circle_shapes.at(0), entity);

        //     if (aabb.top > e.window.settings.size.y / 2.f * 0.01)
        //         e.DestroyEntity(entity);
        // }

        for (auto [entity, rgbd, position, shape, coll] : e.Select<doge::RigidBody, doge::Position, doge::CircleShape, doge::CompoundCollider>().EntitiesAndComponents())
        {
            auto aabb = doge::global::GetAABB(shape, entity);

            if (aabb.top > e.window.settings.size.y / 2.f * 0.01)
                e.DestroyEntity(entity);
        }

        for (auto [sprite] : e.Select<doge::Sprite>().Components())
        {
            if (doge::global::GetAABB(sprite, e).top > e.window.settings.size.y / 2.f * 0.01)
                e.DestroyEntity(sprite.GetEntity());
        }

        for (auto [tag, position, shape, rot, scale] : e.Select<doge::Tag, doge::Position, doge::RectangleShape, doge::Rotation, doge::Scale>().Components())
        {
            auto& comp = e.GetEntity(std::stoi(tag.tags.begin().operator*())).GetComponent<doge::Sprite>();
            doge::Rectf aabb = doge::global::GetAABB(comp, comp.GetEntity(), e);
            doge::global::SetPosition(position, aabb.GetPosition());
            doge::global::SetRotation(rot, 0);
            doge::global::SetScale(scale, doge::Vec2f::One);
            shape.size.Set(aabb.width, aabb.height);
        }

        if (mouse_down_id != -1)
        {
            for (auto [tag, line, pos] : e.Select<doge::Tag>().Where(
                [](doge::EntityID entity, const doge::Tag& tag)
                {
                    return *tag.tags.begin() == "line";
                }).Select<doge::PolygonShape, doge::Position>().Components())
            {
                line.vertices.at(1).position = e.window.MapPixelToCoords(e.window.window_io.GetMousePosition(), cam.GetComponent<doge::Camera>()) - pos.position;
            }
        }
    }

    void FixedUpdate(doge::Engine& e, doge::DeltaTime dt)
    {
    }

    void Finish(doge::Engine& e)
    {
        phy.release();
        doge::default_functions::Finish(e);
    }

    int Main()
    {
        srand(time(0));
        doge::Engine e;
        e.window.settings.title = "doge test";
        e.window.settings.fps = 120;
        e.window.settings.v_sync = true;
        e.scenes.fixed_time_step = 20;

        e.window.SetIcon("assets/textures/missing_texture.png");

        doge::GameLoopFunctions test;
        test.start = Start;
        test.update = Update;
        test.fixed_update = FixedUpdate;
        test.finish = Finish;

        e.AddScene("Test", test);

        e.StartScene("Test", doge::Window::Settings({ 1280, 720 }, "doge test", 60, doge::Window::Settings::Style::Default));

        return 0;
    }
};