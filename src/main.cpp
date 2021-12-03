#include <iostream>

#include <doge/doge.hpp>
#include <ctime>

namespace TestScene
{
    int count = 0;

    void Start(doge::Engine& e)
    {
        count = 0;
        // e.AddCamera(doge::Camera{ .port = doge::Rectf(0, 0, 0.5, 0.5)     , .scale = doge::Vec2f(2, 2) });
        // e.AddCamera(doge::Camera{ .port = doge::Rectf(0, 0.5, 0.5, 0.5)   , .scale = doge::Vec2f(2, 2) });
        // e.AddCamera(doge::Camera{ .port = doge::Rectf(0.5, 0, 0.5, 0.5)   , .scale = doge::Vec2f(2, 2) });
        // e.AddCamera(doge::Camera{ .port = doge::Rectf(0.5, 0.5, 0.5, 0.5) , .scale = doge::Vec2f(2, 2) });

        auto cam = e.AddCamera(doge::Camera{ .scale = doge::Vec2f(2, 2) });

        for (auto i = 0; i < 50; ++i)
        {
            auto my_shape = e.AddEntity("Test", "test");
            my_shape.AddComponent<doge::Position>(0, 0);
            my_shape.AddComponent<doge::Velocity>(rand() % 100 / 100.f - 0.5f, rand() % 100 / 100.f - 0.5f);

            my_shape.AddComponent<doge::CircleShape>(doge::CircleShape
            {
                .radius = 10,
                .origin = doge::Vec2f(10, 10),
                .color = doge::Color(0x00FF0088),
            });
        }
    }

    void Update(doge::Engine& e, doge::DeltaTime dt)
    {
        for (auto [ett, pos, vel] : e.Select<doge::Position, doge::Velocity>().EntitiesAndComponents())
        {
            pos.position += vel.velocity * dt;

            if (pos.position.x < -(float)e.GetVideoSettings().resolution.x / 2) vel.velocity.x = std::abs(vel.velocity.x);
            else if (pos.position.x > (float)e.GetVideoSettings().resolution.x / 2) vel.velocity.x = -std::abs(vel.velocity.x);
            if (pos.position.y < -(float)e.GetVideoSettings().resolution.y / 2) vel.velocity.y = std::abs(vel.velocity.y);
            else if (pos.position.y > (float)e.GetVideoSettings().resolution.y / 2) vel.velocity.y = -std::abs(vel.velocity.y);
        }

        if (++count > 300)
        {
            e.RestartScene();
        }
    }
};

int main()
{
    srand(time(0));
    doge::Engine e;
    e.SetTitle("doge test");
    e.SetFrameRate(60);
    e.AddScene("Test", TestScene::Start, TestScene::Update);

    e.StartScene("Test", doge::VideoSettings(1280, 720, doge::VideoSettings::Mode::Windowed));

    return 0;
}