#include <iostream>

#include <doge/doge.hpp>
#include <doge/extensions/physics/physics.hpp>
#include <box2d/box2d.h>
#include <ctime>
#include <algorithm>

namespace TestScene
{
    doge::Entity AddGreenRect(doge::Engine& e)
    {
        auto my_shape = e.AddEntity("Test", "test");

        my_shape.AddComponent(doge::RigidBody
        {
            .type = doge::RigidBody::Type::Dynamic, 
        });

        my_shape.AddComponent(doge::ConvexCollider
        {
            .points =
            {
                { 10.f, 10.f }, { -10.f, 10.f }, { -20.f, -10.f }, { 20.f, -10.f }
            },
            .density = 1.f,
            .friction = .3f,
            .restitution = .5f,
        });

        my_shape.AddComponent<doge::Position>();
        my_shape.AddComponent<doge::Rotation>(std::fmod(rand(), doge::math::pi * 2));
        // my_shape.AddComponent<doge::Velocity>(doge::Vec2f(std::fmod(rand(), 10) - 5, std::fmod(rand(), 10) - 5).Normalized() * 200.f);
        // my_shape.AddComponent<doge::AngularVelocity>(20);

        doge::physics::BodyInit init;
        init.velocity = doge::Vec2f(std::fmod(rand(), 10) - 5, std::fmod(rand(), 10) - 5).Normalized() * 200.f * 0.01;
        init.angular_velocity = 20.f;
        doge::physics::SetBodyInit(my_shape, init);

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

            my_shape.AddComponent(doge::CompoundCollider
            {
                .circle_colliders = 
                {
                    doge::CircleCollider
                    {
                        .radius = 10.f,
                        .origin = { 15, 0 },
                        .density = 1.f,
                        .friction = .3f,
                        .restitution = .5f,
                    }
                },
                .rectangle_colliders = 
                {
                    doge::RectangleCollider
                    {
                        .size = { 30, 8 },
                        .origin = { 0, 0 },
                        .density = 1.f,
                        .friction = .3f,
                        .restitution = .5f,
                    }
                },
            });

            my_shape.AddComponent<doge::Position>(-i * 0.5, 0);
            my_shape.AddComponent<doge::Rotation>(std::fmod(rand(), doge::math::pi * 2));
            // my_shape.AddComponent<doge::Velocity>(doge::Vec2f(std::fmod(rand(), 10) - 5, std::fmod(rand(), 10) - 5).Normalized() * 50.f);
            // my_shape.AddComponent<doge::AngularVelocity>(20);
            my_shape.AddComponent<doge::Scale>(0.01, 0.01);

            doge::physics::BodyInit init;
            init.velocity = doge::Vec2f(std::fmod(rand(), 10) - 5, std::fmod(rand(), 10) - 5).Normalized() * 50.f * 0.01;
            init.angular_velocity = 20.f;
            doge::physics::SetBodyInit(my_shape, init);

            auto my_comp = my_shape.AddComponent(doge::CompoundSprite
            {
                .circle_shapes = 
                {
                    doge::CircleShape
                    {
                        .radius = 10.f,
                        .origin = doge::Vec2f(25.f, 10.f),
                        .color = doge::Color(0x0000FF88),
                    },
                },
                .rectangle_shapes =
                {
                    doge::RectangleShape
                    {
                        .size = { 30, 8 },
                        .origin = { 15, 4 },
                        .color = doge::Color(0x0000FF88),
                    },
                },
            });

            auto aabb = e.AddEntity();
            aabb.SetParent(my_shape);
            aabb.AddComponent<doge::Tag>(std::set{ std::to_string(my_shape.id) });
            aabb.AddComponent(doge::RectangleShape
            {
                .color = doge::Color::Transparent(),
                .outline_color = doge::Color::White(),
                .outline_thickness = 0.01f,
            });
            aabb.AddComponent<doge::Position>();
            aabb.AddComponent<doge::Scale>();
            aabb.AddComponent<doge::Rotation>();
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
            .size = { 100, 100 },
        });
        my_sprite.AddComponent<doge::Scale>(0.01, 0.01);
    }

    int count = 0;
    doge::Entity cam;

    void Start(doge::Engine& e)
    {
        count = 0;

        cam = e.AddCamera();
        cam.AddComponent<doge::Position>();
        cam.AddComponent<doge::Rotation>();
        cam.AddComponent<doge::Scale>(0.01, 0.01);

        auto ground = e.AddEntity();
        ground.AddComponent<doge::RigidBody>(doge::RigidBody::Type::Static);
        ground.AddComponent(doge::EdgeCollider
        {
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
            .color = doge::Color::Red(),
        });

        ground.AddComponent<doge::Scale>(0.01, 0.01);

        auto my_custom_shape = e.AddEntity();
        my_custom_shape.AddComponent(doge::PolygonShape
        {
            .type = doge::PolygonShape::Type::Triangles,
            .vertices = 
            {
                doge::PolygonShape::Vertex({ 0, 0 }, doge::Color::Red()),
                doge::PolygonShape::Vertex({ 100, 100 }, doge::Color::Green()),
                doge::PolygonShape::Vertex({ 100, 0 }, doge::Color::Blue()),
            },
            .origin = { 50, 50 },
            .texture_id = "missing_texture",
        });
        my_custom_shape.AddComponent(doge::RigidBody(doge::RigidBody::Kinematic));
        my_custom_shape.AddComponent(doge::ConvexCollider
        {
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

        doge::physics::SetBodyInit(my_custom_shape.id, doge::physics::BodyInit{ .angular_velocity = 5.f });

        AddBlocks(e);

        e.events.on_key_pressed += [&e](const doge::event::Key& key)
        {
            if (key.key == doge::event::Key::Code::Space)
            {
                auto scrnshot = e.window.TakeScreenshot();

                scrnshot.ToFile("screenshot.png");
            }
        };
    }

    void Update(doge::Engine& e, doge::DeltaTime dt)
    {
        if (++count > 100)
        {
            AddBlocks(e);
            count = 0;
        }

        for (auto [entity, rgbd, scale, position, convex, coll] : e.Select<doge::RigidBody, doge::Scale, doge::Position, doge::ConvexShape, doge::ConvexCollider>().EntitiesAndComponents())
        {
            if (doge::global::GetAABB(convex).top > e.window.settings.size.y / 2.f * 0.01)
                e.DestroyEntity(entity);
        }

        for (auto [entity, rgbd, position, shape, coll] : e.Select<doge::RigidBody, doge::Position, doge::CompoundSprite, doge::CompoundCollider>().EntitiesAndComponents())
        {
            auto aabb = doge::global::GetAABB(shape.circle_shapes.at(0), entity);

            if (aabb.top > e.window.settings.size.y / 2.f * 0.01)
                e.DestroyEntity(entity);
        }

        for (auto [sprite] : e.Select<doge::Sprite>().Components())
        {
            if (doge::global::GetAABB(sprite).top > e.window.settings.size.y / 2.f * 0.01)
                e.DestroyEntity(sprite.GetEntity());
        }

        for (auto [tag, position, shape, rot, scale] : e.Select<doge::Tag, doge::Position, doge::RectangleShape, doge::Rotation, doge::Scale>().Components())
        {
            auto& comp = e.GetEntity(std::stoi(tag.tags.begin().operator*())).GetComponent<doge::CompoundSprite>();
            doge::Rectf aabb = doge::global::GetAABB(comp.circle_shapes.at(0), comp.GetEntity());
            doge::global::SetPosition(position, doge::cast::PosFromRect(aabb));
            doge::global::SetRotation(rot, 0);
            doge::global::SetScale(scale, doge::Vec2f::One());
            shape.size.Set(aabb.width, aabb.height);
        }
    }

    void FixedUpdate(doge::Engine& e, doge::DeltaTime dt)
    {
    }

    void Finish(doge::Engine& e)
    {
        doge::default_functions::Finish(e);
    }
};

int main()
{
    srand(time(0));
    doge::Engine e;
    e.window.settings.title = "doge test";
    e.window.settings.fps = 120;
    e.window.settings.v_sync = true;
    e.scenes.fixed_time_step = 20;

    e.window.SetIcon("assets/textures/missing_texture.png");

    doge::GameLoopFunctions test;
    test.start = TestScene::Start;
    test.update = TestScene::Update;
    test.fixed_update = TestScene::FixedUpdate;
    test.finish = TestScene::Finish;

    e.AddScene("Test", test);

    doge::physics::Enable(e);

    e.StartScene("Test", doge::Window::Settings({ 1280, 720 }, "doge test", 60, doge::Window::Settings::Style::Default));

    doge::physics::Disable(e);

    return 0;
}