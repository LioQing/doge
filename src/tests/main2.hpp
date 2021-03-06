#include <iostream>
#include <doge/doge.hpp>
#include <doge/extensions/nine_slice.hpp>

namespace main2
{
    std::unique_ptr<doge::nine_slice::NineSlice> ns = nullptr;

    void Start(doge::Engine& engine)
    {
        ns = std::make_unique<doge::nine_slice::NineSlice>(engine);

        engine.assets.LoadTexture("crate", "test2.png").first->second.SetRepeated(true);
        engine.assets.LoadTexture("crate_center", "test2.png", doge::Recti(4, 5, 23, 23)).first->second.SetRepeated(true);

        ns->LoadTexture("crate", "test2.png", doge::Recti(4, 5, 5, 4));
        ns->SetRepeated("crate", true);

        doge::Entity cam = engine.AddCamera();

        doge::Entity crate = engine.AddEntity();
        crate.AddComponent(doge::Position(0, 0));
        crate.AddComponent(doge::Sprite{ .texture_id = "crate", .texture_rectangle = doge::Recti(0, 0, 64, 64), .size = doge::Vec2f(100, 100) });

        doge::Entity crate_center = engine.AddEntity();
        crate_center.AddComponent(doge::Position(150, 0));
        crate_center.AddComponent(doge::Sprite{ .texture_id = "crate_center", .texture_rectangle = doge::Recti(0, 0, 64, 64), .size = doge::Vec2f(100, 100) });

        doge::Entity crate_center_dup = engine.AddEntity();
        crate_center_dup.AddComponent(doge::Position(300, 0));
        crate_center_dup.AddComponent(doge::Sprite{ .texture_id = "crate", .texture_rectangle = doge::Recti(4, 5, 23, 23), .size = doge::Vec2f(100, 100) });

        doge::Entity crate_9sliced = engine.AddEntity();
        crate_9sliced.AddComponent(doge::Position(-150, 0));
        crate_9sliced.AddComponent(doge::nine_slice::Sprite(
            "crate",
            doge::Vec2f(200, 200),
            doge::Vec2i(46, 46),
            doge::Rectf(),
            doge::Vec2f(100, 100)
        ));

        doge::Entity crate_9sliced2 = engine.AddEntity();
        crate_9sliced2.AddComponent(doge::Position(-500, 0));
        crate_9sliced2.AddComponent(doge::nine_slice::Sprite::CreateByTile(
            *ns,
            "crate",
            doge::Vec2f(100, 100),
            doge::Vec2u(2, 2),
            doge::nine_slice::Sprite::BorderThickness::TileScale,
            doge::Vec2f(100, 100)
        ));

        doge::Entity ind = engine.AddEntity();
        ind.AddComponent(doge::Position(-150, 0));
        ind.AddComponent(doge::CircleShape
        {
            .radius = 5.f,
            .origin = doge::Vec2f(5.f, 5.f),
            .color = doge::Color::Green,
        });
    }

    void Update(doge::Engine& engine, doge::DeltaTime dt)
    {
        std::cout << 1000 / dt << std::endl;
    }

    void Finish(doge::Engine& engine)
    {
        ns.release();
    }

    int Main()
    {
        doge::Engine engine;
        
        doge::GameLoopFunctions glf;
        glf.start           = Start;
        glf.update          = Update;
        glf.finish          = Finish;

        engine.AddScene("test2", glf);

        engine.StartScene("test2", doge::Window::Settings
        {
            .size = doge::Vec2u(1280, 720),
            .title = "test2",
            .fps = 0,
        });

        return 0;
    }
}

