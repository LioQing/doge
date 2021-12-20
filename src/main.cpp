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
            my_shape.AddComponent<doge::AngularVelocity>(std::fmod(rand() / 1000.f, doge::math::pi * 4) - doge::math::pi * 2);

            my_shape.AddComponent(doge::ConvexShape
            {
                .points = { doge::Vec2f(-7, -10), doge::Vec2f(7, -10), doge::Vec2f(5, 10), doge::Vec2f(0, 10) },
                //.points = { doge::Vec2f(-7, -10), doge::Vec2f(7, -10), doge::Vec2f(10, 10), doge::Vec2f(0, 10) },
                //.points = { doge::Vec2f(0, 10), doge::Vec2f(5, 10), doge::Vec2f(7, -10), doge::Vec2f(-7, -10) },
                .color = doge::Color(0x00FF0088),
            });
        }
    }

    int count = 0;

    void Start(doge::Engine& e)
    {
        count = 0;

        auto cam = e.AddCamera();
        cam.AddComponent<doge::Position>();
        cam.AddComponent<doge::Rotation>();

        auto ground = e.AddEntity();
        ground.AddComponent<doge::RigidBody>(doge::RigidBody::Type::Static);
        ground.AddComponent(doge::EdgeCollider
        {
            // .points = shape: \/
            // { 
            //     doge::Vec2f(600, 0), 
            //     doge::Vec2f(600, 1), 
            //     doge::Vec2f(300, 100), 
            //     doge::Vec2f(0, 1),
            //     doge::Vec2f(0, 0),
            // },
            .points = // shape: |/\|
            { 
                doge::Vec2f(-static_cast<float>(e.GetVideoSettings().resolution.x) / 2.f, -100),
                doge::Vec2f(-static_cast<float>(e.GetVideoSettings().resolution.x) / 2.f, 0),
                doge::Vec2f(0, -50), 
                doge::Vec2f(e.GetVideoSettings().resolution.x / 2.f, 0), 
                doge::Vec2f(e.GetVideoSettings().resolution.x / 2.f, -100), 
            },
            //.size = doge::Vec2f(e.GetVideoSettings().resolution.x, 10.0f),
        });
        ground.AddComponent<doge::Position>(0.f, e.GetVideoSettings().resolution.y / 2.f - 50.f);

        ground.AddComponent(doge::RectangleShape
        {
            .size = doge::Vec2f(e.GetVideoSettings().resolution.x, 10.0f),
            .origin = doge::Vec2f(e.GetVideoSettings().resolution.x / 2.f, 5.f),
            .color = doge::Color::Red(),
        });

        AddBlocks(e);
    }

    void Update(doge::Engine& e, doge::DeltaTime dt)
    {
        if (++count > 300)
        {
            AddBlocks(e);
            // for (auto [vel] : e.Select<doge::Velocity>().Components())
            //     vel.velocity += doge::Vec2f(0, -50);
            count = 0;
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