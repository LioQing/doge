#include <iostream>
#include <doge/doge.hpp>
#include <doge/extensions/nine_slice.hpp>

using namespace doge;

namespace Test2
{
    void Start(Engine& engine)
    {
        engine.assets.LoadTexture("crate", "test2.png").first->second.SetRenderOptions(TextureEx::RenderOptions::Repeated);
        engine.assets.LoadTexture("crate_center", "test2.png", Recti(4, 5, 23, 23)).first->second.SetRenderOptions(TextureEx::RenderOptions::Repeated);

        nine_slice::LoadTexture(engine.assets, "crate", "test2.png", Recti(4, 5, 5, 4));
        nine_slice::SetRepeated(engine.assets, "crate", true);

        Entity cam = engine.AddCamera();

        Entity crate = engine.AddEntity();
        crate.AddComponent(Position(0, 0));
        crate.AddComponent(Sprite{ .texture_id = "crate", .texture_rectangle = Recti(0, 0, 64, 64), .size = Vec2f(100, 100) });

        Entity crate_center = engine.AddEntity();
        crate_center.AddComponent(Position(150, 0));
        crate_center.AddComponent(Sprite{ .texture_id = "crate_center", .texture_rectangle = Recti(0, 0, 64, 64), .size = Vec2f(100, 100) });

        Entity crate_center_dup = engine.AddEntity();
        crate_center_dup.AddComponent(Position(300, 0));
        crate_center_dup.AddComponent(Sprite{ .texture_id = "crate", .texture_rectangle = Recti(4, 5, 23, 23), .size = Vec2f(100, 100) });

        Entity crate_9sliced = engine.AddEntity();
        crate_9sliced.AddComponent(Position(-150, 0));
        nine_slice::Add9SliceSprite(
            engine.assets,
            crate_9sliced,
            "crate",
            Recti(0, 0, 64, 64),
            Vec2f(100, 100)
        );
    }

    void Update(Engine& engine, DeltaTime dt)
    {
    }
}

int main()
{
    Engine engine;
    engine.window.settings.fps = 120;
    
    GameLoopFunctions glf;
    glf.start           = Test2::Start;
    glf.update          = Test2::Update;

    engine.AddScene("test2", glf);

    engine.StartScene("test2", Window::Settings{ .size = Vec2u(1280, 720), .title = "test2" });

    return 0;
}