#include <iostream>

#include <doge/doge.hpp>
#include <ctime>

namespace TestScene
{
    void Start(doge::Engine& e)
    {
        // e.AddCamera(doge::Camera{ .port = doge::Rectf(0, 0, 0.5, 0.5)     , .scale = doge::Vec2f(2, 2) });
        // e.AddCamera(doge::Camera{ .port = doge::Rectf(0, 0.5, 0.5, 0.5)   , .scale = doge::Vec2f(2, 2) });
        // e.AddCamera(doge::Camera{ .port = doge::Rectf(0.5, 0, 0.5, 0.5)   , .scale = doge::Vec2f(2, 2) });
        // e.AddCamera(doge::Camera{ .port = doge::Rectf(0.5, 0.5, 0.5, 0.5) , .scale = doge::Vec2f(2, 2) });

        e.AddCamera();

        for (auto i = 0; i < 50; ++i)
        {
            auto& my_shape = e.AddEntity("Test", "test");
            my_shape.AddComponent<doge::Position>();
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

            if (pos.position.x < -(float)e.GetVideoSettings().resolution.x / 2) e.DestroyEntity(ett);//vel.velocity.x = std::abs(vel.velocity.x);
            else if (pos.position.x > (float)e.GetVideoSettings().resolution.x / 2) vel.velocity.x = -std::abs(vel.velocity.x);
            if (pos.position.y < -(float)e.GetVideoSettings().resolution.y / 2) vel.velocity.y = std::abs(vel.velocity.y);
            else if (pos.position.y > (float)e.GetVideoSettings().resolution.y / 2) vel.velocity.y = -std::abs(vel.velocity.y);
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
    e.Start("Test", doge::VideoSettings(1280, 720, doge::VideoSettings::Mode::Windowed));

    return 0;
}