#include <iostream>

#include <doge/doge.hpp>
#include <doge/extensions/physics.hpp>
#include <box2d/box2d.h>
#include <ctime>

namespace TestScene
{
    void AddBlocks(doge::Engine& e)
    {
        int last = -1;
        for (auto i = 0; i < 20; ++i)
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
                .density = 1.f,
                .restitution = .5f,
                .friction = .3f,
            });

            my_shape.AddComponent(doge::ConvexCollider
            {
                .points = { doge::Vec2f(-7, -10), doge::Vec2f(7, -10), doge::Vec2f(5, 10), doge::Vec2f(0, 10) },
                //.points = { doge::Vec2f(-7, -10), doge::Vec2f(7, -10), doge::Vec2f(10, 10), doge::Vec2f(0, 10) },
                //.points = { doge::Vec2f(0, 10), doge::Vec2f(5, 10), doge::Vec2f(7, -10), doge::Vec2f(-7, -10) },
            });

            my_shape.AddComponent<doge::Position>(i * 30, 0);
            my_shape.AddComponent<doge::Rotation>(std::fmod(rand(), doge::math::pi * 2));
            my_shape.AddComponent<doge::Velocity>(doge::Vec2f(std::fmod(rand(), 10) - 5, std::fmod(rand(), 10) - 5).Normalized() * 50.f);
            my_shape.AddComponent<doge::AngularVelocity>(20);

            my_shape.AddComponent(doge::ConvexShape
            {
                .points = { doge::Vec2f(-7, -10), doge::Vec2f(7, -10), doge::Vec2f(5, 10), doge::Vec2f(0, 10) },
                //.points = { doge::Vec2f(-7, -10), doge::Vec2f(7, -10), doge::Vec2f(10, 10), doge::Vec2f(0, 10) },
                //.points = { doge::Vec2f(0, 10), doge::Vec2f(5, 10), doge::Vec2f(7, -10), doge::Vec2f(-7, -10) },
                .color = doge::Color(0x00FF0088),
            });
        }

        for (auto i = 1; i < 21; ++i)
        {
            auto my_shape = e.AddEntity();

            my_shape.AddComponent(doge::RigidBody
            {
                .type = doge::RigidBody::Type::Dynamic, 
                .density = 1.f,
                .restitution = .5f,
                .friction = .3f,
            });

            my_shape.AddComponent(doge::CircleCollider
            {
                .radius = 10.f,
            });

            my_shape.AddComponent<doge::Position>(-i * 30, 0);
            my_shape.AddComponent<doge::Rotation>(std::fmod(rand(), doge::math::pi * 2));
            my_shape.AddComponent<doge::Velocity>(doge::Vec2f(std::fmod(rand(), 10) - 5, std::fmod(rand(), 10) - 5).Normalized() * 50.f);
            my_shape.AddComponent<doge::AngularVelocity>(20);

            my_shape.AddComponent(doge::CircleShape
            {
                .radius = 10.f,
                .origin = doge::Vec2f(10.f, 10.f),
                .color = doge::Color(0x0000FF88),
            });
        }
    }

    int count = 0;
    doge::Entity cam;

    void Start(doge::Engine& e)
    {
        count = 0;

        cam = e.AddCamera();
        cam.AddComponent<doge::Position>();
        cam.AddComponent<doge::Rotation>();

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
                doge::Vec2f(-static_cast<float>(e.GetVideoSettings().resolution.x) / 2.f + 100, 0),
                doge::Vec2f(0, -100), 
                doge::Vec2f(e.GetVideoSettings().resolution.x / 2.f - 100, 0), 
            },
            //.size = doge::Vec2f(e.GetVideoSettings().resolution.x, 10.0f),
        });
        ground.AddComponent<doge::Position>(0.f, e.GetVideoSettings().resolution.y / 2.f - 50.f);

        ground.AddComponent(doge::ConvexShape
        {
            .points = // shape: /\.
            { 
                doge::Vec2f(-static_cast<float>(e.GetVideoSettings().resolution.x) / 2.f + 100, 0),
                doge::Vec2f(0, -100), 
                doge::Vec2f(e.GetVideoSettings().resolution.x / 2.f - 100, 0), 
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

        for (auto [entity, rgbd, position] : e.Select<doge::RigidBody, doge::Position>().EntitiesAndComponents())
        {
            if (position.position.y > e.GetVideoSettings().resolution.y / 2.f)
                e.DestroyEntity(entity);
        }

        //std::cout << 1000/dt << std::endl;
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
    e.SetTitle("doge test");
    e.SetFrameRate(60);
    e.SetFixedTimeStep(20);

    doge::GameLoopFunctions test;
    test.start = TestScene::Start;
    test.update = TestScene::Update;
    test.fixed_update = TestScene::FixedUpdate;
    test.finish = TestScene::Finish;

    e.AddScene("Test", test);

    doge::physics::Enable(e);

    e.StartScene("Test", doge::VideoSettings(1280, 720, 60, doge::VideoSettings::Mode::Windowed));

    doge::physics::Disable(e);

    return 0;
}