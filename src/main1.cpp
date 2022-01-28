#include <iostream>
#include <cmath>

#include <doge/doge.hpp>
#include <doge/extensions/physics.hpp>
#include <doge/extensions/GUI.hpp>

using namespace doge;

namespace ParticleSim
{
    std::unique_ptr<GUI> gui = nullptr;
    int shoot_particle = -1;
    Vec2f shoot_particle_position;
    Vec2f shoot_mouse_position;
    Entity shoot_line;
    Component<Camera>* cam_comp;

    Entity AddParticle(Engine& engine, const Vec2f& position)
    {
        Entity particle = engine.AddEntity();

        particle.AddComponent(Tag::Create("particle"));

        particle.AddComponent(CircleShape
        {
            .radius = .2f,
            .origin = { .2f, .2f },
            //.texture_id = "icons",
            .texture_rectangle = Recti(0, 0, 32, 32),
        });

        particle.AddComponent<RigidBody>(RigidBody::Type::Dynamic, true);
        particle.AddComponent(CircleCollider
        {
            .rigid_body_entity = particle,
            .radius = .2f,
            .friction = 0.4f,
            .restitution = 0.8f,
        });

        particle.AddComponent<Position>(position);
        particle.AddComponent<Rotation>();

        return particle;
    }

    void Start(Engine& engine)
    {
        gui = std::make_unique<GUI>(engine);

        {
            auto [itr, success] = engine.assets.LoadTexture("icons", "test.png");
            if (success)
            {
                itr->second.atlas_rectangles = std::unordered_map<std::string, Recti>{{ "base", Recti(0, 0, 32, 32) }};
            }
        }

        engine.assets.LoadTexture("crate", "test2.png");
        engine.assets.LoadTexture("crate_center", "test2.png", Recti(4, 5, 23, 23));

        // cam
        Entity cam = engine.AddCamera(Camera{ .size = Vec2f(12.8, 7.2), .render_order = 1 });
        cam_comp = &cam.GetComponent<Camera>();
        cam.AddComponent(Layer::Create(1, 0, -1));

        // smaller cam
        Entity subcam = engine.AddCamera(Camera{ .size = Vec2f(12.8, 7.2), .port = Rectf(0, 0, 0.25, 0.25), .render_order = 0 });
        subcam.AddComponent(Layer::Create(0, -1));
        
        // square for testing layer rendering
        Entity foreground = engine.AddEntity();
        foreground.AddComponent(Layer::Create(1));
        foreground.AddComponent(Position(-0.5, 0));
        foreground.AddComponent(RectangleShape
        {
            .size = Vec2f(0.7, 0.7),
            .origin = Vec2f(0.35, 0.35),
            .color = Color::Yellow,
        });

        Entity midground = engine.AddEntity();
        midground.AddComponent(Position(0, 0));
        midground.AddComponent(RectangleShape
        {
            .size = Vec2f(0.5, 0.5),
            .origin = Vec2f(0.25, 0.25),
            .color = Color::Cyan,
        });

        Entity background = engine.AddEntity();
        background.AddComponent(Layer::Create(-1));
        background.AddComponent(Position(0.5, 0));
        background.AddComponent(RectangleShape
        {
            .size = Vec2f(0.9, 0.9),
            .origin = Vec2f(0.45, 0.45),
            .color = Color::Magenta,
        });

        // rounded rectangle
        Entity rr = engine.AddEntity();
        rr.AddComponent(Position(1, 0));
        rr.AddComponent(ConvexShape
        {
            .points = math::RoundedRectangle(Vec2f(1, 1), 0.08),
            .origin = Vec2f(0.5, 0.5),
        });
        rr.AddComponent(RigidBody
        {
            .type = RigidBody::Type::Static,
        });
        rr.AddComponent(RectangleCollider
        {
            .rigid_body_entity = rr,
            .size = Vec2f(1, 1),
        });

        // wall
        Entity wall = engine.AddEntity();

        wall.AddComponent<RigidBody>(RigidBody::Type::Static);
        wall.AddComponent(EdgeCollider
        {
            .rigid_body_entity = wall,
            .points = 
            {
                Vec2f(-6.4, -3.6),
                Vec2f(-6.4, 3.6),
                Vec2f(6.4, 3.6),
                Vec2f(6.4, -3.6),
            },
            .is_loop = true,
            .friction = 0.1f,
            .restitution = 0.8f,
        });

        // particles
        for (std::size_t i = 1; i <= 20; ++i)
        {
            AddParticle(engine, engine.window.MapPixelToCoords({ 0, 0 }, *cam_comp) + Vec2f(i * .5f, .5f));
        }

        // shoot actions
        shoot_line = engine.AddEntity();
        shoot_line.AddComponent(Tag::Create("line"));
        shoot_line.AddComponent<Position>();
        shoot_line.AddComponent(PolygonShape
        { 
            .type = PolygonShape::Lines, 
            .vertices = 
            { 
                PolygonShape::Vertex(Vec2f(0, 0), Color::Transparent), 
                PolygonShape::Vertex(Vec2f(0, 0), Color::Transparent), 
            } 
        });

        engine.events.on_mouse_button_pressed += [&](const event::MouseButton& event)
        {
            if (event.button == event::MouseButton::Button::Left)
            {
                shoot_mouse_position = engine.window.MapPixelToCoords(event.position, *cam_comp);
                for (auto entity : engine.Select<Tag>()
                    .Where([](const Entity& _, const Tag& tag)
                    { return *tag.tags.begin() == "particle"; })
                    .Entities())
                {
                    if (!physics::GetCollider(entity).TestPoint(shoot_mouse_position))
                        continue;

                    shoot_line.GetComponent<Position>().position = shoot_mouse_position;
                    for (auto& vertex : shoot_line.GetComponent<PolygonShape>().vertices)
                        vertex.color = Color::White;
                    
                    shoot_particle_position = entity.GetIfHasComponentElseDefault<Position>().position;
                    shoot_particle = entity.id;

                    engine.window.window_io.SetMouseCursor(engine.assets.GetCursor("grab"));

                    return;
                }

                AddParticle(engine, shoot_mouse_position);
            }
            else if (event.button == event::MouseButton::Button::Right)
            {
                shoot_mouse_position = engine.window.MapPixelToCoords(event.position, *cam_comp);
                for (auto entity : engine.Select<Tag>()
                    .Where([](const Entity& _, const Tag& tag)
                    { return *tag.tags.begin() == "particle"; })
                    .Entities())
                {
                    if (!physics::GetCollider(entity).TestPoint(shoot_mouse_position))
                        continue;

                    engine.DestroyEntity(entity);
                    return;
                }
            }
        };

        engine.events.on_mouse_button_released += [&](const event::MouseButton& event)
        {
            if (event.button == event::MouseButton::Button::Left)
            {
                if (shoot_particle == -1) return;

                if (!engine.HasEntity(shoot_particle) || !doge::physics::HasBody(shoot_particle))
                    return;
                
                auto impulse = (shoot_mouse_position - engine.window.MapPixelToCoords(event.position, *cam_comp));
                physics::GetBody(shoot_particle).ApplyImpulse(impulse, shoot_mouse_position);

                for (auto& vertex : shoot_line.GetComponent<PolygonShape>().vertices)
                    vertex.color = Color::Transparent;

                // engine.GetEntity(shoot_particle).GetComponent<EntityInfo>().enabled = false;

                shoot_particle = -1;

                engine.assets.GetSound("shoot").SetVolume(std::clamp(impulse.Magnitude() * 10.f, 0.f, 100.f));
                engine.assets.GetSound("shoot").Play();

                engine.window.window_io.SetMouseCursor(engine.assets.GetCursor("normal"));
            }
        };

        // gui elements
        gui->AddCamera("gui_cam");
        
        Button& button0 = gui->AddElement<Button>("button0", "gui_cam");
        button0.SetPosition(Vec2f(-300, 0));
        button0.SetTextFont("arial");
        button0.SetText(U"Testing");
        auto appear = button0.GetTextAppearance();
        appear.fill_color = Color::Red;
        button0.SetTextAppearance(appear);

        button0.on_pressed += [](){ std::cout << "Pressed" << std::endl; };
        button0.on_released += [](){ std::cout << "Released" << std::endl; };
        button0.on_mouse_entered += [](){ std::cout << "Entered" << std::endl; };
        button0.on_mouse_left += [](){ std::cout << "Left" << std::endl; };
        button0.on_clicked += [](){ std::cout << "Clicked" << std::endl; };

        // text
        {
            Entity text = engine.AddEntity();
            text.AddComponent(Layer::Create(1));
            text.AddComponent(Position(0, -1.6));
            text.AddComponent(Scale(0.01, 0.01));
            text.AddComponent(Text
            {
                .font_id = "arial",
                .string = U"mundob\nmundo\nAVAVAVAV",
                .align = Text::Align::Left,
                .character_appearances = std::map<std::size_t, Text::Appearance>
                {
                    std::pair<std::size_t, Text::Appearance>(0,  Text::Appearance{ .style = Text::Style::Regular }),
                    std::pair<std::size_t, Text::Appearance>(7,  Text::Appearance{ .style = Text::Style::Regular, .fill_color = Color::Red }),
                    std::pair<std::size_t, Text::Appearance>(8,  Text::Appearance{ .style = Text::Style::Regular, .fill_color = Color::Yellow }),
                    std::pair<std::size_t, Text::Appearance>(9,  Text::Appearance{ .style = Text::Style::Regular, .fill_color = Color::Green }),
                    std::pair<std::size_t, Text::Appearance>(10, Text::Appearance{ .style = Text::Style::Regular, .fill_color = Color::Cyan }),
                    std::pair<std::size_t, Text::Appearance>(11, Text::Appearance{ .style = Text::Style::Regular, .fill_color = Color::Blue }),
                    std::pair<std::size_t, Text::Appearance>(12, Text::Appearance{ }),
                },
            });
        }
        {
            Entity text = engine.AddEntity();
            text.AddComponent(Layer::Create(1));
            text.AddComponent(Position(0, -2.2));
            text.AddComponent(Scale(0.01, 0.01));
            text.AddComponent(Text
            {
                .font_id = "arial",
                .string = U"mundob\nmundo\nAVAVAVAV",
                .align = Text::Align::Center,
                .character_appearances = std::map<std::size_t, Text::Appearance>
                {
                    std::pair<std::size_t, Text::Appearance>(0,  Text::Appearance{ .style = Text::Style::Regular }),
                    std::pair<std::size_t, Text::Appearance>(7,  Text::Appearance{ .style = Text::Style::Regular, .fill_color = Color::Red }),
                    std::pair<std::size_t, Text::Appearance>(8,  Text::Appearance{ .style = Text::Style::Regular, .fill_color = Color::Yellow }),
                    std::pair<std::size_t, Text::Appearance>(9,  Text::Appearance{ .style = Text::Style::Regular, .fill_color = Color::Green }),
                    std::pair<std::size_t, Text::Appearance>(10, Text::Appearance{ .style = Text::Style::Regular, .fill_color = Color::Cyan }),
                    std::pair<std::size_t, Text::Appearance>(11, Text::Appearance{ .style = Text::Style::Regular, .fill_color = Color::Blue }),
                    std::pair<std::size_t, Text::Appearance>(12, Text::Appearance{ }),
                },
            });
        }
        {
            Entity text = engine.AddEntity();
            text.AddComponent(Layer::Create(1));
            text.AddComponent(Position(0, -2.8));
            text.AddComponent(Scale(0.01, 0.01));
            text.AddComponent(Text
            {
                .font_id = "arial",
                .string = U"mundob\nmundo\nAVAVAVAV",
                .align = Text::Align::Right,
                .character_appearances = std::map<std::size_t, Text::Appearance>
                {
                    std::pair<std::size_t, Text::Appearance>(0,  Text::Appearance{ .style = Text::Style::Regular }),
                    std::pair<std::size_t, Text::Appearance>(7,  Text::Appearance{ .style = Text::Style::Regular, .fill_color = Color::Red }),
                    std::pair<std::size_t, Text::Appearance>(8,  Text::Appearance{ .style = Text::Style::Regular, .fill_color = Color::Yellow }),
                    std::pair<std::size_t, Text::Appearance>(9,  Text::Appearance{ .style = Text::Style::Regular, .fill_color = Color::Green }),
                    std::pair<std::size_t, Text::Appearance>(10, Text::Appearance{ .style = Text::Style::Regular, .fill_color = Color::Cyan }),
                    std::pair<std::size_t, Text::Appearance>(11, Text::Appearance{ .style = Text::Style::Regular, .fill_color = Color::Blue }),
                    std::pair<std::size_t, Text::Appearance>(12, Text::Appearance{ }),
                },
            });
        }
    }

    void Update(Engine& engine, DeltaTime dt)
    {
        if (shoot_particle != -1)
        {
            for (auto [tag, line, pos] : engine.Select<Tag>()
                .Where([](EntityID entity, const Tag& tag)
                { return *tag.tags.begin() == "line"; })
                .Select<PolygonShape, Position>().Components())
            {
                line.vertices.at(1).position = engine.window.MapPixelToCoords(engine.window.window_io.GetMousePosition(), *cam_comp) - pos.position;
            }
        }

        //std::cout << 1000.f / dt << std::endl;
    }

    void FixedUpdate(Engine& engine, DeltaTime dt)
    {
        for (auto [entity, tag, position] : engine.Select<Tag>()
            .Where([](const Entity& _, const Tag& tag)
            { return *tag.tags.begin() == "particle"; })
            .Select<Position>().EntitiesAndComponents())
        {
            if (entity != shoot_particle)
                continue;
            
            physics::Body body = physics::GetBody(entity);

            Vec2f diff = shoot_particle_position - position.position;
            body.SetVelocity(diff);
        }
    }

    void Finish(Engine& engine)
    {
        gui.release();
    }
}

int main()
{
    Engine engine;
    engine.window.settings.fps = 120;
    engine.window.settings.msaa_level = 4;
    engine.window.settings.size = Vec2u(1280, 720);
    engine.window.settings.title = "Particle Simulation";

    engine.assets.AddSound("shoot", "shoot", "shoot.wav");

    engine.assets.LoadCursor("normal", io::Cursor::Type::Arrow);
    engine.assets.LoadCursor("grab", io::Cursor::Type::Hand);

    physics::Enable(engine); 
    physics::SetGravity(Vec2f(0, 9.8));

    GameLoopFunctions glf;
    glf.start           = ParticleSim::Start;
    glf.update          = ParticleSim::Update;
    glf.fixed_update    = ParticleSim::FixedUpdate;
    glf.finish          = ParticleSim::Finish;

    engine.AddScene("particle_sim", glf);

    engine.StartScene("particle_sim");

    return 0;
}