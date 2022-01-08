#include <iostream>

#include <doge/doge.hpp>
#include <doge/extensions/physics.hpp>
#include <box2d/box2d.h>
#include <ctime>
#include <algorithm>

namespace TestScene
{
    void AddBlocks(doge::Engine& e)
    {
        int last = -1;
        for (auto i = 0; i < 10; ++i)
        {
            auto my_shape = e.AddEntity("Test", "test");

            if (last != -1)
            {
                my_shape.SetParent(last);
                my_shape.RemoveParent();
            }
            last = my_shape.id;

            my_shape.AddComponent(doge::RigidBody
            {
                .type = doge::RigidBody::Type::Dynamic, 
            });

            // my_shape.AddComponent(doge::ConvexCollider
            // {
            //     .points = { { -7, -10 }, { 7, -10 }, { 12, 10 }, { -12, 10 } },
            //     .density = 1.f,
            //     .restitution = .5f,
            //     .friction = .3f,
            // });

            my_shape.AddComponent(doge::RectangleCollider
            {
                .size = { 20, 40 },
                .density = 1.f,
                .restitution = .5f,
                .friction = .3f,
            });

            my_shape.AddComponent<doge::Position>(i * 50, 0);
            my_shape.AddComponent<doge::Rotation>(std::fmod(rand(), doge::math::pi * 2));
            my_shape.AddComponent<doge::Velocity>(doge::Vec2f(std::fmod(rand(), 10) - 5, std::fmod(rand(), 10) - 5).Normalized() * 200.f);
            my_shape.AddComponent<doge::AngularVelocity>(20);
            my_shape.AddComponent<doge::Scale>(2, 2);

            // auto& rect = my_shape.AddComponent(doge::ConvexShape
            // {
            //     .points = { { -7, -10 }, { 7, -10 }, { 12, 10 }, { -12, 10 } },
            //     .color = doge::Color(0x00FF0088),
            // });

            my_shape.AddComponent(doge::RectangleShape
            {
                .size = { 20, 40 },
                .origin = { 10, 20 },
                //.color = doge::Color(0x00FF0088),
                .outline_color = doge::Color::Green(),
                .outline_thickness = 2.f,
                .texture_id = "missing_texture",
                .texture_rectangle = { 0, 0, 30, 30 },
            });
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
                        .restitution = .5f,
                        .friction = .3f,
                    }
                },
                .rectangle_colliders = 
                {
                    doge::RectangleCollider
                    {
                        .size = { 30, 8 },
                        .origin = { 0, 0 },
                        .density = 1.f,
                        .restitution = .5f,
                        .friction = .3f,
                    }
                },
            });

            my_shape.AddComponent<doge::Position>(-i * 50, 0);
            my_shape.AddComponent<doge::Rotation>(std::fmod(rand(), doge::math::pi * 2));
            my_shape.AddComponent<doge::Velocity>(doge::Vec2f(std::fmod(rand(), 10) - 5, std::fmod(rand(), 10) - 5).Normalized() * 50.f);
            my_shape.AddComponent<doge::AngularVelocity>(20);

            my_shape.AddComponent(doge::CompoundSprite
            {
                .circle_shapes = 
                {
                    doge::CircleShape
                    {
                        .radius = 10.f,
                        .origin = doge::Vec2f(25.f, 10.f),
                        //.color = doge::Color(0x0000FF88),
                        .outline_color = doge::Color::Blue(),
                        .outline_thickness = 2.f,
                        .texture_id = "missing_texture",
                        .texture_rectangle = { 0, 0, 30, 30 },
                    },
                },
                .rectangle_shapes =
                {
                    doge::RectangleShape
                    {
                        .size = { 30, 8 },
                        .origin = { 15, 4 },
                        //.color = doge::Color(0x0000FF88),
                        .outline_color = doge::Color::Blue(),
                        .outline_thickness = 2.f,
                        .texture_id = "missing_texture",
                        .texture_rectangle = { 0, 0, 30, 30 },
                    },
                },
            });
        }

        auto my_sprite = e.AddEntity();
        my_sprite.AddComponent(doge::Sprite
        {
            .texture_rectangle = { 0, 0, 100, 100 },
            .origin = { 50, 50 },
        });
        my_sprite.AddComponent(doge::RigidBody{ .type = doge::RigidBody::Type::Dynamic });
        my_sprite.AddComponent<doge::Position>(0, -200);
        my_sprite.AddComponent<doge::Rotation>();
        my_sprite.AddComponent(doge::RectangleCollider
        {
            .size = { 100, 100 },
        });
    }

    int count = 0;
    doge::Entity cam;

    void Start(doge::Engine& e)
    {
        count = 0;

        cam = e.AddCamera();
        cam.AddComponent<doge::Position>();
        cam.AddComponent<doge::Rotation>();
        // cam.AddComponent<doge::Scale>(2, 2);

        auto ground = e.AddEntity();
        ground.AddComponent<doge::RigidBody>(doge::RigidBody::Type::Static);
        ground.AddComponent(doge::EdgeCollider
        {
            // .points = // shape: \/
            // { 
            //     doge::Vec2f(-static_cast<float>(e.GetVideoSettings().resolution.x) / 2.f + 100, -100), 
            //     doge::Vec2f(0, 0), 
            //     doge::Vec2f(e.GetVideoSettings().resolution.x / 2.f - 100, -100),
            // },
            .points = // shape: /\.
            { 
                doge::Vec2f(-static_cast<float>(e.window_settings.resolution.x) / 2.f + 100, 0),
                doge::Vec2f(0, -100), 
                doge::Vec2f(e.window_settings.resolution.x / 2.f - 100, 0), 
            },
            //.size = doge::Vec2f(e.GetVideoSettings().resolution.x, 10.0f),
        });
        ground.AddComponent<doge::Position>(0.f, e.window_settings.resolution.y / 2.f - 50.f);

        ground.AddComponent(doge::ConvexShape
        {
            .points = // shape: /\.
            { 
                doge::Vec2f(-static_cast<float>(e.window_settings.resolution.x) / 2.f + 100, 0),
                doge::Vec2f(0, -100), 
                doge::Vec2f(e.window_settings.resolution.x / 2.f - 100, 0), 
            },
            .color = doge::Color::Red(),
        });

        AddBlocks(e);
    }

    void Update(doge::Engine& e, doge::DeltaTime dt)
    {
        if (++count > 100)
        {
            AddBlocks(e);
            count = 0;
        }

        for (auto [entity, rgbd, scale, velocity, position, convex, coll] : e.Select<doge::RigidBody, doge::Scale, doge::Velocity, doge::Position, doge::RectangleShape, doge::RectangleCollider>().EntitiesAndComponents())
        {
            if (doge::global::GetAABB(convex).top > e.window_settings.resolution.y / 2.f)
                e.DestroyEntity(entity);
            
            //scale.scale = doge::Vec2f::One() * std::clamp(100.f / velocity.velocity.Magnitude(), 0.1f, 2.f);
            //coll.apply_changes = true;
        }

        for (auto [entity, rgbd, position, shape, coll, velocity] : e.Select<doge::RigidBody, doge::Position, doge::CompoundSprite, doge::CompoundCollider, doge::Velocity>().EntitiesAndComponents())
        {
            if (position.position.y - shape.circle_shapes.at(0).radius > e.window_settings.resolution.y / 2.f)
            {
                e.DestroyEntity(entity);
            }
            
            //shape.circle_shapes.at(0).radius = std::clamp(500.f / velocity.velocity.Magnitude(), 0.f, 50.f);
            //shape.circle_shapes.at(0).origin = { shape.circle_shapes.at(0).radius + 15, shape.circle_shapes.at(0).radius };

            //coll.circle_colliders.at(0).radius = shape.circle_shapes.at(0).radius;
            //coll.circle_colliders.at(0).apply_changes = true;
        }

        for (auto [sprite] : e.Select<doge::Sprite>().Components())
        {
            if (doge::global::GetAABB(sprite).top > e.window_settings.resolution.y / 2.f)
                e.DestroyEntity(sprite.GetEntity());
        }

        std::cout << 1000/dt << std::endl;
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
    e.window_settings.title = "doge test";
    e.window_settings.fps = 60;
    e.SetFixedTimeStep(20);

    doge::GameLoopFunctions test;
    test.start = TestScene::Start;
    test.update = TestScene::Update;
    test.fixed_update = TestScene::FixedUpdate;
    test.finish = TestScene::Finish;

    e.AddScene("Test", test);

    doge::physics::Enable(e);

    e.StartScene("Test", doge::WindowSettings(1280, 720, "doge test", 60, doge::WindowSettings::Mode::Windowed));

    doge::physics::Disable(e);

    return 0;
}