#include <iostream>

#include <doge/doge.hpp>
#include <doge/extensions/physics.hpp>
#include <box2d/box2d.h>
#include <ctime>

namespace TestScene
{
    int count = 0;
    b2World world(b2Vec2(0.f, 50.f));
    std::vector<b2Body*> bodies;
    b2Body* groundBody;

    void Start(doge::Engine& e)
    {
        count = 0;

        auto cam = e.AddCamera();
        cam.AddComponent<doge::Position>();
        cam.AddComponent<doge::Rotation>();

        auto ground = e.AddEntity();
        ground.AddComponent<doge::RigidBody>(doge::RigidBody::Type::Static);
        ground.AddComponent<doge::RectangleCollider>(doge::RectangleCollider
        {
            .size = doge::Vec2f(e.GetVideoSettings().resolution.x, 10.0f),
        });
        ground.AddComponent<doge::Position>(0.f, e.GetVideoSettings().resolution.y / 2.f - 50.f);

        ground.AddComponent<doge::RectangleShape>(doge::RectangleShape
        {
            .size = doge::Vec2f(e.GetVideoSettings().resolution.x, 10.0f),
            .origin = doge::Vec2f(e.GetVideoSettings().resolution.x / 2.f, 5.f),
            .color = doge::Color::Red(),
        });

        int last = -1;
        for (auto i = 0; i < 20; ++i)
        {
            auto my_shape = e.AddEntity("Test", "test");

            if (last != -1)
            {
                my_shape.SetParent(last);
                my_shape.RemoveParent();
            }
            else
            {
                //cam.SetParent(my_shape);
            }
            last = my_shape.id;

            my_shape.AddComponent<doge::RigidBody>(doge::RigidBody
            {
                .type = doge::RigidBody::Type::Dynamic, 
                .density = 1.f,
                .restitution = .5f,
            });
            my_shape.AddComponent<doge::ConvexCollider>(doge::ConvexCollider
            {
                //.points = { doge::Vec2f(-7, -10), doge::Vec2f(7, -10), doge::Vec2f(5, 10), doge::Vec2f(0, 10) },
                .points = { doge::Vec2f(-7, -10), doge::Vec2f(7, -10), doge::Vec2f(10, 10), doge::Vec2f(0, 10) },
                //.points = { doge::Vec2f(0, 10), doge::Vec2f(5, 10), doge::Vec2f(7, -10), doge::Vec2f(-7, -10) },
            });

            my_shape.AddComponent<doge::Position>(i * 30, 0);
            my_shape.AddComponent<doge::Rotation>(std::fmod(rand(), doge::math::pi * 2));
            my_shape.AddComponent<doge::Velocity>(doge::Vec2f(std::fmod(rand(), 10) - 5, std::fmod(rand(), 10) - 5).Normalized() * 50.f);

            my_shape.AddComponent<doge::ConvexShape>(doge::ConvexShape
            {
                //.points = { doge::Vec2f(-7, -10), doge::Vec2f(7, -10), doge::Vec2f(5, 10), doge::Vec2f(0, 10) },
                .points = { doge::Vec2f(-7, -10), doge::Vec2f(7, -10), doge::Vec2f(10, 10), doge::Vec2f(0, 10) },
                //.points = { doge::Vec2f(0, 10), doge::Vec2f(5, 10), doge::Vec2f(7, -10), doge::Vec2f(-7, -10) },
                .color = doge::Color(0x00FF0088),
            });
        }
    }

    void Update(doge::Engine& e, doge::DeltaTime dt)
    {
        if (++count > 1000)
        {
            e.RestartScene();
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
    e.SetTitle("doge test");
    e.SetFrameRate(60);
    e.SetFixedTimeStep(2);

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