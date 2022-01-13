#include <iostream>
#include <cmath>

#include <doge/doge.hpp>
#include <doge/extensions/physics/physics.hpp>

using namespace doge;

namespace ParticleSim
{
    int grab_particle = -1;
    Vec2f grab_particle_position;
    Vec2f grab_mouse_position;
    Entity grab_line;
    Component<Camera>* cam_comp;

    void Start(Engine& engine)
    {
        // cam
        Entity cam = engine.AddCamera(Vec2f(128, 72));
        cam_comp = &cam.GetComponent<Camera>();

        // wall
        Entity wall = engine.AddEntity();
        Vec2u window_size = engine.window.settings.size;

        wall.AddComponent<RigidBody>(RigidBody::Type::Static);
        wall.AddComponent(EdgeCollider
        {
            .points = 
            {
                engine.window.MapPixelToCoords(Vec2i(0,             0            ), *cam_comp),
                engine.window.MapPixelToCoords(Vec2i(0,             window_size.y), *cam_comp),
                engine.window.MapPixelToCoords(Vec2i(window_size.x, window_size.y), *cam_comp),
                engine.window.MapPixelToCoords(Vec2i(window_size.x, 0            ), *cam_comp),
            },
            .is_loop = true,
            .friction = 0.1f,
            .restitution = 0.8f,
        });

        // particles
        for (std::size_t i = 1; i <= 20; ++i)
        {
            Entity particle = engine.AddEntity();

            particle.AddComponent<Tag>(std::set{ std::string("particle") });

            particle.AddComponent(CircleShape
            {
                .radius = 2.f,
                .origin = { 2.f, 2.f },
                .color = Color::White(),
            });

            particle.AddComponent<RigidBody>(RigidBody::Type::Dynamic);
            particle.AddComponent(CircleCollider
            {
                .radius = 2.f,
                .friction = 0.1f,
                .restitution = 0.8f,
            });

            particle.AddComponent<Position>(engine.window.MapPixelToCoords({ 0, 0 }, *cam_comp) + Vec2f(i * 5.f, 5.f));
        }

        // grab actions
        grab_line = engine.AddEntity();
        grab_line.AddComponent<Tag>(std::set{ std::string("line") });
        grab_line.AddComponent<Position>();
        grab_line.AddComponent(PolygonShape
        { 
            .type = PolygonShape::Lines, 
            .vertices = 
            { 
                PolygonShape::Vertex(Vec2f(0, 0), Color::Transparent()), 
                PolygonShape::Vertex(Vec2f(0, 0), Color::Transparent()), 
            } 
        });

        engine.events.on_mouse_button_pressed += [&](const event::MouseButton& event)
        {
            if (event.button == event::MouseButton::Button::Left)
            {
                for (auto entity : engine.Select<Tag>()
                    .Where([](const Entity& _, const Tag& tag)
                    { return *tag.tags.begin() == "particle"; })
                    .Entities())
                {
                    grab_mouse_position = engine.window.MapPixelToCoords(event.position, *cam_comp);
                    if (!physics::GetCollider(entity).TestPoint(grab_mouse_position))
                        continue;

                    grab_line.GetComponent<Position>().position = grab_mouse_position;
                    for (auto& vertex : grab_line.GetComponent<PolygonShape>().vertices)
                        vertex.color = Color::White();
                    
                    grab_particle_position = entity.GetIfHasComponentElseDefault<Position>().position;
                    grab_particle = entity.id;

                    return;
                }
            }
        };

        engine.events.on_mouse_button_released += [&](const event::MouseButton& event)
        {
            if (event.button == event::MouseButton::Button::Left)
            {
                if (grab_particle == -1) return;

                if (!engine.HasEntity(grab_particle) || !doge::physics::HasBody(grab_particle))
                    return;
                
                physics::GetBody(grab_particle).ApplyImpulse(
                    (grab_mouse_position - engine.window.MapPixelToCoords(event.position, *cam_comp)) * 100.f,
                    grab_mouse_position
                );

                for (auto& vertex : grab_line.GetComponent<PolygonShape>().vertices)
                    vertex.color = Color::Transparent();

                grab_particle = -1;
            }
        };
    }

    void Update(Engine& engine, DeltaTime dt)
    {
        if (grab_particle != -1)
        {
            for (auto [tag, line, pos] : engine.Select<Tag>().Where(
                [](EntityID entity, const Tag& tag)
                {
                    return *tag.tags.begin() == "line";
                }).Select<PolygonShape, Position>().Components())
            {
                line.vertices.at(1).position = engine.window.MapPixelToCoords(engine.window.window_io.GetMousePosition(), *cam_comp) - pos.position;
            }
        }
    }

    void FixedUpdate(Engine& engine, DeltaTime dt)
    {
        for (auto [entity, tag, position] : engine.Select<Tag>()
            .Where([](const Entity& _, const Tag& tag)
            { return *tag.tags.begin() == "particle"; })
            .Select<Position>().EntitiesAndComponents())
        {
            if (entity != grab_particle)
                continue;
            
            physics::Body body = physics::GetBody(entity);

            Vec2f diff = grab_particle_position - position.position;
            body.SetVelocity(diff);
        }
    }
}

int main()
{
    Engine engine;

    physics::Enable(engine);

    GameLoopFunctions glf;
    glf.start           = ParticleSim::Start;
    glf.update          = ParticleSim::Update;
    glf.fixed_update    = ParticleSim::FixedUpdate;

    engine.AddScene("particle_sim", glf);

    engine.StartScene("particle_sim", Window::Settings{ .size = Vec2u(1280, 720), .title = "Particle Simulation" });

    return 0;
}