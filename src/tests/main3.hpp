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

        doge::Component<doge::CircleShape>* circle_comp;
        doge::gui::Text* text;
        doge::gui::Image* image;
        doge::gui::WindowEx* window;

        std::size_t window_count;

        SceneB(doge::Engine& engine) : engine(engine), gui(engine)
        {
        }

        void AddWindow(const doge::Vec2f& position)
        {
            window = &gui.AddElement<doge::gui::WindowEx>("my_window" + std::to_string(window_count), "my_cam");
            window->SetAlign(doge::Vec2f(0, 0));
            window->SetSize(doge::Vec2f(400, 200));
            window->SetPosition(doge::Vec2f(-100, -100));
            window->SetTitleBar(true);

            auto& button = window->AddElement<doge::gui::Button>("my_button" + std::to_string(window_count));
            button.SetPosition(-window->GetSize() / 2.f);
            button.SetAlign(doge::Vec2f::Zero);
            button.SetOrigin(-window->GetBorderThickness().GetPosition());

            auto& text = button.GetText();
            text.SetString(U"Lock");

            button.GetClickable().on_clicked +=
            [this](doge::io::Mouse::Button button)
            {
                if (this->circle_comp->color == doge::Color::Red)
                {
                    this->circle_comp->color = doge::Color::Green;
                    this->text->SetAlign(doge::gui::Align::Top);
                    this->image->SetTextureRectangle(doge::Recti(32, 0, 32, 32));
                    this->window->SetDraggable(true);
                }
                else
                {
                    this->circle_comp->color = doge::Color::Red;
                    this->text->SetAlign(doge::gui::Align::Bottom);
                    this->image->SetTextureRectangle(doge::Recti(0, 0, 32, 32));
                    this->window->SetDraggable(false);
                }
            };

            window_count++;
        }

        void Start(doge::Engine& engine)
        {
            window_count = 0;

            engine.assets.LoadTexture("test", "test.png");
            gui.GetNineSlice().LoadTexture("test2", "test2.png", doge::Recti(4, 5, 5, 4));
            gui.GetNineSlice().SetRepeated("test2", true);

            // gui cam
            gui.AddCamera("my_cam");
            AddWindow(doge::Vec2f::Zero);

            // button
            // auto& button = gui.AddElement<doge::gui::Button>("my_button", "my_cam");
            // button.SetPosition(doge::Vec2f(0, 300));

            // text
            text = &gui.AddElement<doge::gui::Text>("my_text", "my_cam");
            text->SetPosition(doge::Vec2f(0, -150));
            text->SetTextAlign(doge::Text::Align::Center);
            text->SetAlign(doge::gui::Align::Bottom);

            // image
            image = &gui.AddElement<doge::gui::Image>("my_image", "my_cam");
            image->SetTextureID("test");
            image->SetTextureRectangle(doge::Recti(0, 0, 32, 32));
            image->SetPosition(doge::Vec2f(300, 0));

            // ns image
            auto& ns_image = gui.AddElement<doge::gui::NSImage>("my_ns_image", "my_cam");
            ns_image.SetTextureID("test2");
            ns_image.SetSize(doge::Vec2f(200, 200));
            ns_image.SetCenterTextureSize(doge::Vec2i(46, 46));
            ns_image.SetPosition(doge::Vec2f(-150, 0));

            // circle in gui
            doge::Entity circle_entity = engine.AddEntity();

            circle_comp = &circle_entity.AddComponent(doge::CircleShape
            {
                .radius = 5.f,
                .origin = { 5.f, 5.f },
                .color = doge::Color::Red,
            });

            circle_entity.AddComponent<doge::Position>(0, -50);
            circle_entity.AddComponent(doge::Layer::Create(gui.GetCameraLayer("doge_gui_window_my_window0")));

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
            // auto e = gui.GetElementBelowCursor();
            // if (e)
            //     std::cout << e->GetID() << std::endl;
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
        engine.window.SetFrameRate(120);
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