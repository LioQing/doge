#pragma once

#include <ctime>
#include <doge/doge.hpp>
#include <doge/extensions/physics.hpp>
#include <doge/extensions/gui.hpp>

namespace main3
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
        doge::gui::GUI gui;

        struct Window
        {
            bool occupied = false;
            std::int32_t layer = 0;
            doge::Vec2f pos;
        };

        std::array<Window, 20> windows;

        SceneB(doge::Engine& engine) : engine(engine), gui(engine)
        {
        }

        void AddWindow()
        {
            int win_i = -1;
            for (auto i = 0; i < windows.size(); ++i)
            {
                if (windows[i].occupied)
                    continue;

                win_i = i;
                break;
            }

            if (win_i == -1)
            {
                std::cout << "Max window count reached." << std::endl;
                return;
            }
            
            windows[win_i].occupied = true;

            auto& window = gui.AddElement<doge::gui::WindowEx>("my_window" + std::to_string(win_i), "my_cam");
            window.SetAlign(doge::Vec2f(1, 1));
            window.SetSize(doge::Vec2f(400, 200));
            window.SetPosition(windows[win_i].pos);
            window.SetLayer(windows[win_i].layer);
            window.SetTitleBar(true);
            window.SetResizable(true);
            window.SetCloseButton(true);
            window.SetDraggable(true);
            window.SetScrollable(true);
            window.GetScrollable().SetAreaUnlimited(false);
            window.GetScrollable().SetArea(doge::Rectf(0, 0, 0, 50));

            gui.GetElementComponent("my_window" + std::to_string(win_i)).OnRemoval([this, win_i]()
            {
                windows[win_i].occupied = false;
            });

            auto& button = window.AddElement<doge::gui::Button>("my_button" + std::to_string(win_i));
            button.SetSize(doge::Vec2f(100, 100));
            button.GetClickable().SetCornerRadius(25);
            button.SetPosition(doge::Vec2f(64, 64));
            button.SetHasText(true);
            button.GetText().SetString(U"Off");

            button.GetClickable().on_clicked +=
            [&](const doge::Vec2f&, doge::io::Mouse::Button button_clicked)
            {
                if (button_clicked == doge::io::Mouse::Button::Left)
                {
                    if (button.GetText().GetString() == U"Off")
                    {
                        button.GetText().SetString(U"On");
                    }
                    else
                    {
                        button.GetText().SetString(U"Off");
                    }
                }
            };
        }

        void Start(doge::Engine& engine)
        {
            engine.assets.LoadTexture("test", "test.png");
            gui.GetNineSlice().LoadTexture("test2", "test2.png", doge::Recti(4, 5, 5, 4));
            gui.GetNineSlice().SetRepeated("test2", true);

            // gui cam
            auto& cam = gui.AddCamera("my_cam");
            cam.SetLayer(doge::Layer::CreateRange(32, 34));

            for (auto i = 0; i < windows.size(); ++i)
            {
                for (auto j = 0; j < 4; ++j)
                    cam.GetLayer().layers.emplace(34 + j + i * 8);

                windows[i].occupied = false;
                windows[i].layer = 34 + i * 8;
                windows[i].pos = i * 10 * doge::Vec2f::One;
            }

            // button
            auto& button = gui.AddElement<doge::gui::Button>("my_image", "my_cam");
            button.Set9Slice(false);
            button.GetImage().SetTextureID("test");
            button.SetSize(doge::Vec2f(32, 32));
            button.GetImage().SetTextureRectangle(doge::Recti(0, 0, 32, 32));
            button.SetPosition(doge::Vec2f(300, 0));

            button.GetClickable().on_clicked.AddListener("spawn_window",
            [&](const doge::Vec2f&, doge::io::Mouse::Button button_clicked)
            {
                if (button_clicked == doge::io::Mouse::Button::Left)
                {
                    AddWindow();
                }
            });

            // ns image
            auto& ns_image = gui.AddElement<doge::gui::NSImage>("my_ns_image", "my_cam");
            ns_image.SetTextureID("test2");
            ns_image.SetSize(doge::Vec2f(200, 200));
            ns_image.SetCenterTextureSize(doge::Vec2i(46, 46));
            //ns_image.SetPosition(doge::Vec2f(-150, 0));

            // slider
            {
                auto& slider = gui.AddElement<doge::gui::Slider>("my_slider0", "my_cam");
                slider.SetPosition(doge::Vec2f(100, 0));
                slider.SetMinValue(0);
                slider.SetMaxValue(3);
                slider.SetAlign(doge::Vec2f::Zero);
                slider.SetOrigin(doge::Vec2f(-100, -200));

                slider.on_value_changed += [](float val){ std::cout << val << std::endl; };
            }
            {
                auto& slider = gui.AddElement<doge::gui::Slider>("my_slider1", "my_cam");
                slider.SetPosition(doge::Vec2f(100, 0));
                slider.SetMinValue(0);
                slider.SetMaxValue(3);
                slider.SetAlign(doge::Vec2f::Zero);
            }

            

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
        
        SceneA a(engine);
        engine.AddScene("a", doge::GameLoopFunctions::Create(a, &SceneA::Start, &SceneA::Update, &SceneA::FixedUpdate, &SceneA::Finish));

        SceneB b(engine);
        engine.AddScene("b", doge::GameLoopFunctions::Create(b, &SceneB::Start, &SceneB::Update, &SceneB::FixedUpdate, &SceneB::Finish));

        engine.StartScene("a");

        return 0;
    }
}