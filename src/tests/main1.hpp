#include <iostream>
#include <cmath>

#include <doge/doge.hpp>
#include <doge/extensions/physics.hpp>
#include <doge/extensions/gui.hpp>

namespace main1
{
    std::unique_ptr<doge::gui::GUI> gui = nullptr;
    std::unique_ptr<doge::physics::Physics> phy = nullptr;

    int shoot_particle = -1;
    doge::Vec2f shoot_particle_position;
    doge::Vec2f shoot_mouse_position;
    doge::Entity shoot_line;
    doge::Component<doge::Camera>* cam_comp;

    doge::Entity AddParticle(doge::Engine& engine, const doge::Vec2f& position)
    {
        doge::Entity particle = engine.AddEntity();

        particle.AddComponent(doge::Tag::Create("particle"));

        particle.AddComponent(doge::CircleShape
        {
            .radius = .2f,
            .origin = { .2f, .2f },
            //.texture_id = "icons",
            .texture_rectangle = doge::Recti(0, 0, 32, 32),
        });

        particle.AddComponent<doge::physics::RigidBody>(doge::physics::RigidBody::Type::Dynamic, true);
        particle.AddComponent(doge::physics::CircleCollider
        {
            .rigid_body_entity = particle,
            .radius = .2f,
            .friction = 0.4f,
            .restitution = 0.8f,
        });

        particle.AddComponent<doge::Position>(position);
        particle.AddComponent<doge::Rotation>();

        return particle;
    }

    void Start(doge::Engine& engine)
    {
        gui = std::make_unique<doge::gui::GUI>(engine);
        phy = std::make_unique<doge::physics::Physics>(engine);

        {
            auto [itr, success] = engine.assets.LoadTexture("icons", "test.png");
            if (success)
            {
                itr->second.atlas_rectangles = std::unordered_map<std::string, doge::Recti>{{ "base", doge::Recti(0, 0, 32, 32) }};
            }
        }

        engine.assets.LoadTexture("crate", "test2.png");
        engine.assets.LoadTexture("crate_center", "test2.png", doge::Recti(4, 5, 23, 23));

        // cam
        doge::Entity cam = engine.AddCamera(doge::Camera{ .size = doge::Vec2f(12.8, 7.2), .render_order = 1 });
        cam_comp = &cam.GetComponent<doge::Camera>();
        engine.events.on_window_resized += [&](const doge::event::Size& event){ cam_comp->size = event.size.Cast<float>() / 100.f; };
        cam.AddComponent(doge::Layer::Create(1, 0, -1));

        // smaller cam
        doge::Entity subcam = engine.AddCamera(doge::Camera{ .size = doge::Vec2f(12.8, 7.2), .port = doge::Rectf(0, 0, 0.25, 0.25), .render_order = 0 });
        subcam.AddComponent(doge::Layer::Create(0, -1));
        
        // square for testing layer rendering
        doge::Entity foreground = engine.AddEntity();
        foreground.AddComponent(doge::Layer::Create(1));
        foreground.AddComponent(doge::Position(-0.5, 0));
        foreground.AddComponent(doge::RectangleShape
        {
            .size = doge::Vec2f(0.7, 0.7),
            .origin = doge::Vec2f(0.35, 0.35),
            .color = doge::Color::Yellow,
        });

        doge::Entity midground = engine.AddEntity();
        midground.AddComponent(doge::Position(0, 0));
        midground.AddComponent(doge::RectangleShape
        {
            .size = doge::Vec2f(0.5, 0.5),
            .origin = doge::Vec2f(0.25, 0.25),
            .color = doge::Color::Cyan,
        });

        doge::Entity background = engine.AddEntity();
        background.AddComponent(doge::Layer::Create(-1));
        background.AddComponent(doge::Position(0.5, 0));
        background.AddComponent(doge::RectangleShape
        {
            .size = doge::Vec2f(0.9, 0.9),
            .origin = doge::Vec2f(0.45, 0.45),
            .color = doge::Color::Magenta,
        });

        // rounded rectangle
        doge::Entity rr = engine.AddEntity();
        rr.AddComponent(doge::Position(1, 0));
        rr.AddComponent(doge::ConvexShape
        {
            .points = doge::math::RoundedRectangle(doge::Vec2f(1, 1), 0.08),
            .origin = doge::Vec2f(0.5, 0.5),
        });
        rr.AddComponent(doge::physics::RigidBody
        {
            .type = doge::physics::RigidBody::Type::Static,
        });
        rr.AddComponent(doge::physics::RectangleCollider
        {
            .rigid_body_entity = rr,
            .size = doge::Vec2f(1, 1),
        });

        // wall
        doge::Entity wall = engine.AddEntity();

        wall.AddComponent<doge::physics::RigidBody>(doge::physics::RigidBody::Type::Static);
        wall.AddComponent(doge::physics::EdgeCollider
        {
            .rigid_body_entity = wall,
            .points = 
            {
                doge::Vec2f(-6.4, -3.6),
                doge::Vec2f(-6.4, 3.6),
                doge::Vec2f(6.4, 3.6),
                doge::Vec2f(6.4, -3.6),
            },
            .is_loop = true,
            .friction = 0.1f,
            .restitution = 0.8f,
        });

        // particles
        for (std::size_t i = 1; i <= 20; ++i)
        {
            AddParticle(engine, engine.window.MapPixelToCoords({ 0, 0 }, *cam_comp) + doge::Vec2f(i * .5f, .5f));
        }

        // shoot actions
        shoot_line = engine.AddEntity();
        shoot_line.AddComponent(doge::Tag::Create("line"));
        shoot_line.AddComponent<doge::Position>();
        shoot_line.AddComponent(doge::PolygonShape
        { 
            .type = doge::PolygonShape::Lines, 
            .vertices = 
            { 
                doge::PolygonShape::Vertex(doge::Vec2f(0, 0), doge::Color::Transparent), 
                doge::PolygonShape::Vertex(doge::Vec2f(0, 0), doge::Color::Transparent), 
            } 
        });

        engine.events.on_mouse_button_pressed += [&](const doge::event::MouseButton& event)
        {
            if (event.button == doge::event::MouseButton::Button::Left)
            {
                // gui->GetElementEntity("button0").GetComponent<EntityInfo>().enabled = false;
                shoot_mouse_position = engine.window.MapPixelToCoords(event.position, *cam_comp);
                for (auto entity : engine.Select<doge::Tag>()
                    .Where([](const doge::Entity& _, const doge::Tag& tag)
                    { return *tag.tags.begin() == "particle"; })
                    .Entities())
                {
                    if (!phy->GetCollider(entity).TestPoint(shoot_mouse_position))
                        continue;

                    shoot_line.GetComponent<doge::Position>().position = shoot_mouse_position;
                    for (auto& vertex : shoot_line.GetComponent<doge::PolygonShape>().vertices)
                        vertex.color = doge::Color::White;
                    
                    shoot_particle_position = entity.GetIfHasComponentElseDefault<doge::Position>().position;
                    shoot_particle = entity.id;

                    engine.window.window_io.SetMouseCursor(engine.assets.GetCursor("grab"));

                    return;
                }

                AddParticle(engine, shoot_mouse_position);
            }
            else if (event.button == doge::event::MouseButton::Button::Right)
            {
                shoot_mouse_position = engine.window.MapPixelToCoords(event.position, *cam_comp);
                for (auto entity : engine.Select<doge::Tag>()
                    .Where([](const doge::Entity& _, const doge::Tag& tag)
                    { return *tag.tags.begin() == "particle"; })
                    .Entities())
                {
                    if (!phy->GetCollider(entity).TestPoint(shoot_mouse_position))
                        continue;

                    engine.DestroyEntity(entity);
                    return;
                }
            }
        };

        engine.events.on_mouse_button_released += [&](const doge::event::MouseButton& event)
        {
            if (event.button == doge::event::MouseButton::Button::Left)
            {
                // gui->GetElementEntity("button0").GetComponent<EntityInfo>().enabled = true;
                if (shoot_particle == -1) return;

                if (!engine.HasEntity(shoot_particle) || !phy->HasBody(shoot_particle))
                    return;
                
                auto impulse = (shoot_mouse_position - engine.window.MapPixelToCoords(event.position, *cam_comp));
                phy->GetBody(shoot_particle).ApplyImpulse(impulse, shoot_mouse_position);

                for (auto& vertex : shoot_line.GetComponent<doge::PolygonShape>().vertices)
                    vertex.color = doge::Color::Transparent;

                // engine.GetEntity(shoot_particle).GetComponent<EntityInfo>().enabled = false;

                shoot_particle = -1;

                engine.assets.GetSound("shoot").SetVolume(std::clamp(impulse.Magnitude() * 10.f, 0.f, 100.f));
                engine.assets.GetSound("shoot").Play();

                engine.window.window_io.SetMouseCursor(engine.assets.GetCursor("normal"));
            }
        };

        // gui elements
        gui->AddCamera("gui_cam");
        
        doge::gui::Button& button0 = gui->AddElement<doge::gui::Button>("button0", "gui_cam");
        button0.SetPosition(doge::Vec2f(-300, 0));
        button0.SetSize(doge::gui::Button::DefaultSize * doge::Vec2f(1, 2));
        button0.GetText().SetString(U"Hello\nthis is me");

        doge::gui::Window& window = gui->AddElement<doge::gui::Window>("window0", "gui_cam");

        doge::gui::Button& button1 = window.AddElement<doge::gui::Button>("button1");
        button1.GetText().SetString(U"Button In Window");

        button1.GetClickable().on_pressed       += [](doge::io::Mouse::Button){ std::cout << "Pressed" << std::endl; };
        button1.GetClickable().on_released      += [](doge::io::Mouse::Button){ std::cout << "Released" << std::endl; };
        button1.GetClickable().on_mouse_entered += [](){ std::cout << "Entered" << std::endl; };
        button1.GetClickable().on_mouse_left    += [](){ std::cout << "Left" << std::endl; };
        button1.GetClickable().on_clicked       += [](doge::io::Mouse::Button){ std::cout << "Clicked" << std::endl; };

        // text
        {
            doge::Entity text = engine.AddEntity();
            text.AddComponent(doge::Layer::Create(1));
            text.AddComponent(doge::Position(0, -1.6));
            text.AddComponent(doge::Scale(0.01, 0.01));
            text.AddComponent(doge::Text
            {
                .string = U"mundob\nmundo\nAVAVAVAV",
                .align = doge::Text::Align::Left,
                .character_appearances = std::map<std::size_t, doge::Text::Appearance>
                {
                    std::pair<std::size_t, doge::Text::Appearance>(0,  doge::Text::Appearance{ .style = doge::Text::Style::Regular }),
                    std::pair<std::size_t, doge::Text::Appearance>(7,  doge::Text::Appearance{ .style = doge::Text::Style::Regular, .fill_color = doge::Color::Red }),
                    std::pair<std::size_t, doge::Text::Appearance>(8,  doge::Text::Appearance{ .style = doge::Text::Style::Regular, .fill_color = doge::Color::Yellow }),
                    std::pair<std::size_t, doge::Text::Appearance>(9,  doge::Text::Appearance{ .style = doge::Text::Style::Regular, .fill_color = doge::Color::Green }),
                    std::pair<std::size_t, doge::Text::Appearance>(10, doge::Text::Appearance{ .style = doge::Text::Style::Regular, .fill_color = doge::Color::Cyan }),
                    std::pair<std::size_t, doge::Text::Appearance>(11, doge::Text::Appearance{ .style = doge::Text::Style::Regular, .fill_color = doge::Color::Blue }),
                    std::pair<std::size_t, doge::Text::Appearance>(12, doge::Text::Appearance{ }),
                },
            });
        }
        {
            doge::Entity text = engine.AddEntity();
            text.AddComponent(doge::Layer::Create(1));
            text.AddComponent(doge::Position(0, -2.2));
            text.AddComponent(doge::Scale(0.01, 0.01));
            text.AddComponent(doge::Text
            {
                .string = U"mundob\nmundo\nAVAVAVAV",
                .align = doge::Text::Align::Center,
                .character_appearances = std::map<std::size_t, doge::Text::Appearance>
                {
                    std::pair<std::size_t, doge::Text::Appearance>(0,  doge::Text::Appearance{ .style = doge::Text::Style::Regular }),
                    std::pair<std::size_t, doge::Text::Appearance>(7,  doge::Text::Appearance{ .style = doge::Text::Style::Regular, .fill_color = doge::Color::Red }),
                    std::pair<std::size_t, doge::Text::Appearance>(8,  doge::Text::Appearance{ .style = doge::Text::Style::Regular, .fill_color = doge::Color::Yellow }),
                    std::pair<std::size_t, doge::Text::Appearance>(9,  doge::Text::Appearance{ .style = doge::Text::Style::Regular, .fill_color = doge::Color::Green }),
                    std::pair<std::size_t, doge::Text::Appearance>(10, doge::Text::Appearance{ .style = doge::Text::Style::Regular, .fill_color = doge::Color::Cyan }),
                    std::pair<std::size_t, doge::Text::Appearance>(11, doge::Text::Appearance{ .style = doge::Text::Style::Regular, .fill_color = doge::Color::Blue }),
                    std::pair<std::size_t, doge::Text::Appearance>(12, doge::Text::Appearance{ }),
                },
            });
        }
        {
            doge::Entity text = engine.AddEntity();
            text.AddComponent(doge::Layer::Create(1));
            text.AddComponent(doge::Position(0, -2.8));
            text.AddComponent(doge::Scale(0.01, 0.01));
            text.AddComponent(doge::Text
            {
                .string = U"mundob\nmundo\nAVAVAVAV",
                .align = doge::Text::Align::Right,
                .character_appearances = std::map<std::size_t, doge::Text::Appearance>
                {
                    std::pair<std::size_t, doge::Text::Appearance>(0,  doge::Text::Appearance{ .style = doge::Text::Style::Regular }),
                    std::pair<std::size_t, doge::Text::Appearance>(7,  doge::Text::Appearance{ .style = doge::Text::Style::Regular, .fill_color = doge::Color::Red }),
                    std::pair<std::size_t, doge::Text::Appearance>(8,  doge::Text::Appearance{ .style = doge::Text::Style::Regular, .fill_color = doge::Color::Yellow }),
                    std::pair<std::size_t, doge::Text::Appearance>(9,  doge::Text::Appearance{ .style = doge::Text::Style::Regular, .fill_color = doge::Color::Green }),
                    std::pair<std::size_t, doge::Text::Appearance>(10, doge::Text::Appearance{ .style = doge::Text::Style::Regular, .fill_color = doge::Color::Cyan }),
                    std::pair<std::size_t, doge::Text::Appearance>(11, doge::Text::Appearance{ .style = doge::Text::Style::Regular, .fill_color = doge::Color::Blue }),
                    std::pair<std::size_t, doge::Text::Appearance>(12, doge::Text::Appearance{ }),
                },
            });
        }
    }

    void Update(doge::Engine& engine, doge::DeltaTime dt)
    {
        if (shoot_particle != -1)
        {
            for (auto [tag, line, pos] : engine.Select<doge::Tag>()
                .Where([](doge::EntityID entity, const doge::Tag& tag)
                { return *tag.tags.begin() == "line"; })
                .Select<doge::PolygonShape, doge::Position>().Components())
            {
                line.vertices.at(1).position = engine.window.MapPixelToCoords(engine.window.window_io.GetMousePosition(), *cam_comp) - pos.position;
            }
        }

        if (gui->GetElementBelowCursor())
            std::cout << gui->GetElementBelowCursor()->GetID() << std::endl;

        //std::cout << 1000.f / dt << std::endl;
    }

    void FixedUpdate(doge::Engine& engine, doge::DeltaTime dt)
    {
        for (auto [entity, tag, position] : engine.Select<doge::Tag>()
            .Where([](const doge::Entity& _, const doge::Tag& tag)
            { return *tag.tags.begin() == "particle"; })
            .Select<doge::Position>().EntitiesAndComponents())
        {
            if (entity != shoot_particle)
                continue;
            
            doge::physics::Body body = phy->GetBody(entity);

            doge::Vec2f diff = shoot_particle_position - position.position;
            body.SetVelocity(diff);
        }
    }

    void Finish(doge::Engine& engine)
    {
        gui.release();
        phy.release();
    }

    int Main()
    {
        doge::Engine engine;
        engine.window.settings.fps = 120;
        engine.window.settings.msaa_level = 4;
        engine.window.settings.size = doge::Vec2u(1280, 720);
        engine.window.settings.title = "Particle Simulation";
        engine.window.SetBackgroundColor(0xF0F0F0FF);

        engine.assets.AddSound("shoot", "shoot", "shoot.wav");

        engine.assets.LoadCursor("normal", doge::io::Cursor::Type::Arrow);
        engine.assets.LoadCursor("grab", doge::io::Cursor::Type::Hand);

        doge::GameLoopFunctions glf;
        glf.start           = Start;
        glf.update          = Update;
        glf.fixed_update    = FixedUpdate;
        glf.finish          = Finish;

        engine.AddScene("particle_sim", glf);

        engine.StartScene("particle_sim");

        return 0;
    }
}

