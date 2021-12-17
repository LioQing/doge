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
        // e.AddCamera(doge::Camera{ .port = doge::Rectf(0, 0, 0.5, 0.5)     , .scale = doge::Vec2f(2, 2) });
        // e.AddCamera(doge::Camera{ .port = doge::Rectf(0, 0.5, 0.5, 0.5)   , .scale = doge::Vec2f(2, 2) });
        // e.AddCamera(doge::Camera{ .port = doge::Rectf(0.5, 0, 0.5, 0.5)   , .scale = doge::Vec2f(2, 2) });
        // e.AddCamera(doge::Camera{ .port = doge::Rectf(0.5, 0.5, 0.5, 0.5) , .scale = doge::Vec2f(2, 2) });

        auto cam = e.AddCamera();
        cam.AddComponent<doge::Position>();
        cam.AddComponent<doge::Rotation>();
        cam.AddComponent<doge::Scale>(0.5, 0.5);

        b2BodyDef groundBodyDef;
        groundBodyDef.position.Set(0.f, e.GetVideoSettings().resolution.y / 2.f - 50.f);
        groundBody = world.CreateBody(&groundBodyDef);
        b2PolygonShape groundBox;
        groundBox.SetAsBox(e.GetVideoSettings().resolution.x / 2.f, 5.0f);
        groundBody->CreateFixture(&groundBox, 0.0f);

        auto ground = e.AddEntity();
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
            last = my_shape.id;

            b2BodyDef bodyDef;
            bodyDef.type = b2_dynamicBody;
            bodyDef.position.Set(i * 15.f, 0.0f);
            bodyDef.angle = std::fmod(rand(), 3.1415 * 2);
            b2PolygonShape dynamicBox;
            dynamicBox.SetAsBox(5.f, 5.f);
            b2FixtureDef fixtureDef;
            fixtureDef.shape = &dynamicBox;
            fixtureDef.density = 1.0f;
            fixtureDef.friction = 0.3f;

            b2Body* body = world.CreateBody(&bodyDef);
            body->CreateFixture(&fixtureDef);
            bodies.push_back(body);

            my_shape.AddComponent<doge::Position>(i * 15, 0);
            my_shape.AddComponent<doge::Rotation>();

            my_shape.AddComponent<doge::RectangleShape>(doge::RectangleShape
            {
                .size = doge::Vec2f(10, 10),
                .origin = doge::Vec2f(5, 5),
                .color = doge::Color(0x00FF0088),
            });
        }
    }

    void Update(doge::Engine& e, doge::DeltaTime dt)
    {
        if (++count > 500)
        {
            e.RestartScene();
        }
    }

    void FixedUpdate(doge::Engine& e, doge::DeltaTime dt)
    {
        world.Step(dt / 1000.f, 1, 1);
        int i = 0;
        bool isFirst = true;
        for (auto [pos, rot, rect] : e.Select<doge::Position, doge::Rotation, doge::RectangleShape>().Components())
        {
            pos.position = doge::cast::FromB2Vec2(bodies.at(i)->GetPosition());
            rot.rotation = bodies.at(i)->GetAngle();

            rot.GetEntity();

            if (isFirst)
            {
                auto camContainer = e.Select<doge::Camera, doge::Position, doge::Rotation>().Components();
                if (camContainer.size() >= 1)
                {
                    auto [camera, pos1, rot1] = *camContainer.begin();
                    pos1.position = pos.position;
                    rot1.rotation = rot.rotation;
                    isFirst = false;
                }
            }

            i++;
        }
    }

    void Finish(doge::Engine& e)
    {
        for (auto* body : bodies)
        {
            world.DestroyBody(body);
        }
        bodies.clear();
        world.DestroyBody(groundBody);
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
    e.AddScene("Test", TestScene::Start, TestScene::Update, TestScene::FixedUpdate, TestScene::Finish);

    doge::physics::Enable(e);

    e.StartScene("Test", doge::VideoSettings(1280, 720, 60, doge::VideoSettings::Mode::Windowed));

    doge::physics::Disable(e);

    return 0;
}