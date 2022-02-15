#pragma once

#include <ctime>
#include <doge/doge.hpp>
#include <doge/extensions/physics.hpp>
#include <doge/extensions/nine_slice.hpp>

namespace main4
{
    struct SceneA
    {
        doge::Engine& engine;
        doge::physics::Physics phy;

        doge::Component<doge::Camera>* cam_comp;

        const float scale = 0.01f;

        SceneA(doge::Engine& engine) : engine(engine), phy(engine)
        {
        }

        doge::Entity SpawnParticle(const doge::Vec2f& position)
        {
            doge::Entity particle = engine.AddEntity();

            particle.AddComponent(doge::Tag::Create("particle"));

            particle.AddComponent(doge::CircleShape
            {
                .radius = .2f,
                .origin = { .2f, .2f },
                .texture_rectangle = doge::Recti(0, 0, 32, 32),
            });

            particle.AddComponent(doge::physics::CircleCollider
            {
                .rigid_body_entity = particle,
                .radius = .2f,
                .friction = 0.4f,
                .restitution = 0.8f,
            });

            particle.AddComponent<doge::Position>(position);
            particle.AddComponent<doge::physics::RigidBody>(doge::physics::RigidBody::Dynamic, true);

            doge::physics::BodyInit init;
            init.velocity = doge::Vec2f(std::fmod(rand() / 100.f, 10.f) - 5.f, std::fmod(rand() / 100.f, 10.f) - 5.f);

            phy.SetBodyInit(particle, init);

            return particle;
        }

        void Start(doge::Engine& engine)
        {
            // camera
            auto cam_entity = engine.AddCamera(doge::Camera
            {
                .size = engine.window.window_io.GetSize() * scale,
            });

            cam_comp = &cam_entity.GetComponent<doge::Camera>();
            engine.events.on_window_resized.AddListener(
                "cam_resize",
                [this](const doge::event::Size& event)
                {
                    this->cam_comp->size = event.size * this->scale;
                }
            );
            cam_comp->OnRemoval([this]{ this->engine.events.on_window_resized.RemoveListener("cam_resize"); });

            // mouse
            engine.events.on_mouse_button_pressed.AddListener(
                "scene_a",
                [this](const doge::event::MouseButton& event)
                {
                    if (event.button == doge::io::Mouse::Left)
                    {
                        SpawnParticle(this->engine.window.MapPixelToCoords(event.position, *this->cam_comp));
                    }
                    else if (event.button == doge::io::Mouse::Right)
                    {
                        this->engine.RestartScene("b");
                    }
                }
            );
        }

        void Update(doge::Engine& engine, doge::DeltaTime dt)
        {
            for (auto [entity, tag, pos, circle] : engine
                .Select<doge::Tag, doge::Position, doge::CircleShape>()
                .Where([](doge::Entity entity, const doge::Tag& tag, const doge::Position& pos, const doge::CircleShape& circle)
                { return tag.tags.contains("particle"); })
                .EntitiesAndComponents())
            {
                if (pos.position.y - circle.radius > cam_comp->size.y / 2.f)
                {
                    engine.DestroyEntity(entity);
                }
            }
        }

        void FixedUpdate(doge::Engine& engine, doge::DeltaTime dt)
        {
            
        }

        void Finish(doge::Engine& engine)
        {
            engine.events.on_mouse_button_pressed.RemoveListener("scene_a");
        }
    };

    struct SceneB
    {
        doge::Engine& engine;
        doge::nine_slice::NineSlice ns;

        SceneB(doge::Engine& engine) : engine(engine), ns(engine)
        {
        }

        void Start(doge::Engine& engine)
        {
            engine.assets.LoadTexture("crate", "test2.png").first->second.SetRenderOptions(doge::io::Texture::RenderOptions::Repeated);
            engine.assets.LoadTexture("crate_center", "test2.png", doge::Recti(4, 5, 23, 23)).first->second.SetRenderOptions(doge::io::Texture::RenderOptions::Repeated);

            ns.LoadTexture("crate", "test2.png", doge::Recti(4, 5, 5, 4));
            ns.SetRepeated("crate", true);

            doge::Entity cam = engine.AddCamera();
            cam.AddComponent(doge::Layer::Create(0, 1));

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
                ns,
                "crate",
                doge::Vec2f(100, 100),
                doge::Vec2u(2, 2),
                doge::nine_slice::Sprite::BorderThickness::TileScale,
                doge::Vec2f(100, 100)
            ));

            doge::Entity ind = engine.AddEntity();
            ind.AddComponent(doge::Layer::Create(1));
            ind.AddComponent(doge::Position(-150, 0));
            ind.AddComponent(doge::CircleShape
            {
                .radius = 5.f,
                .origin = doge::Vec2f(5.f, 5.f),
                .color = doge::Color::Green,
            });

            engine.events.on_mouse_button_pressed.AddListener(
                "scene_b",
                [this](const doge::event::MouseButton& event)
                {
                    if (event.button == doge::io::Mouse::Right)
                    {
                        this->engine.RestartScene("a");
                    }
                }
            );
        }

        void Update(doge::Engine& engine, doge::DeltaTime dt)
        {
            // auto* e = gui.GetCameraFocused();
            // if (e)
            //     std::cout << e->GetID() << std::endl;

            // if (gui.HasElement("my_window0"))
            // {
            //     std::cout << static_cast<doge::gui::WindowEx&>(gui.GetElement("my_window0")).GetWindowCamera().GetCameraPosition() << std::endl;
            // }
        }

        void FixedUpdate(doge::Engine& engine, doge::DeltaTime dt)
        {

        }

        void Finish(doge::Engine& engine)
        {
            engine.assets.EraseTexture("test");
            engine.events.on_mouse_button_pressed.RemoveListener("scene_b");
        }
    };

    int Main()
    {
        srand(time(NULL));

        doge::Engine engine;
        engine.window.SetFrameRate(0);
        engine.window.settings.size = (doge::Vec2u(1280, 720));
        engine.window.settings.msaa_level = 4;
        
        {
            SceneA a(engine);
            {
                doge::GameLoopFunctions glf;
                glf.start = std::bind(&SceneA::Start, &a, std::placeholders::_1);
                glf.fixed_update = std::bind(&SceneA::FixedUpdate, &a, std::placeholders::_1, std::placeholders::_2);
                glf.update = std::bind(&SceneA::Update, &a, std::placeholders::_1, std::placeholders::_2);
                glf.finish = std::bind(&SceneA::Finish, &a, std::placeholders::_1);

                engine.AddScene("a", glf);
            }

            SceneB b(engine);
            {
                doge::GameLoopFunctions glf;
                glf.start = std::bind(&SceneB::Start, &b, std::placeholders::_1);
                glf.fixed_update = std::bind(&SceneB::FixedUpdate, &b, std::placeholders::_1, std::placeholders::_2);
                glf.update = std::bind(&SceneB::Update, &b, std::placeholders::_1, std::placeholders::_2);
                glf.finish = std::bind(&SceneB::Finish, &b, std::placeholders::_1);

                engine.AddScene("b", glf);
            }

            engine.StartScene("a");
        }

        return 0;
    }
}