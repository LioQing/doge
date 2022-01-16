#include <iostream>
#include <cmath>

#include <doge/doge.hpp>
#include <doge/extensions/physics.hpp>
#include <doge/extensions/fsm.hpp>

using namespace doge;

namespace ParticleSim
{
    int shoot_particle = -1;
    Vec2f shoot_particle_position;
    Vec2f shoot_mouse_position;
    Entity shoot_line;
    Component<Camera>* cam_comp;

    void Start(Engine& engine)
    {
        // cam
        Entity cam = engine.AddCamera(Vec2f(12.8, 7.2));
        cam_comp = &cam.GetComponent<Camera>();

        // wall
        Entity wall = engine.AddEntity();

        wall.AddComponent<RigidBody>(RigidBody::Type::Static);
        wall.AddComponent(EdgeCollider
        {
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
            Entity particle = engine.AddEntity();

            particle.AddComponent(Tag::Create("particle"));

            particle.AddComponent(CircleShape
            {
                .radius = .2f,
                .origin = { .2f, .2f },
                .texture_rectangle = Recti(0, 0, 32, 32),
            });

            particle.AddComponent<RigidBody>(RigidBody::Type::Dynamic, true);
            particle.AddComponent(CircleCollider
            {
                .radius = .2f,
                .friction = 0.4f,
                .restitution = 0.8f,
            });

            particle.AddComponent<Position>(engine.window.MapPixelToCoords({ 0, 0 }, *cam_comp) + Vec2f(i * .5f, .5f));
            particle.AddComponent<Rotation>();
            
            auto& fsm = particle.AddComponent(StateMachines
            (
                {
                    StateMachine // color: 0 - white (still), 1 - red (moving), 2 - blue (grabbed)
                    {
                        .state = 0,
                        .transition = 
                        [&](fsm::State state, Entity entity, Engine& engine, DeltaTime dt)
                        {
                            if (shoot_particle == entity.id)
                                return 2;

                            if (physics::GetBody(entity).GetVelocity().Magnitude() > .01f)
                                return 1;
                            
                            return 0;
                        }
                    },
                    StateMachine // movement: 0 - grab, 1 - free
                    {
                        .state = 0,
                    },
                }
            ));

            fsm.state_machines.at(0).on_entry += 
            [&](fsm::State state, Entity entity, Engine& engine, DeltaTime dt)
            {
                if (state == 0)
                    entity.GetComponent<CircleShape>().color = Color::White();
                else if (state == 1)
                    entity.GetComponent<CircleShape>().color = Color::Red();
                else if (state == 2)
                    entity.GetComponent<CircleShape>().color = Color::Blue();
            };

            fsm.state_machines.at(1).on_entry += 
            [&](fsm::State state, Entity entity, Engine& engine, DeltaTime dt)
            {
                if (state == 0)
                {
                    entity.GetComponent<CircleShape>().texture_id = "";
                }
                else if (state == 1)
                {
                    entity.GetComponent<CircleShape>().texture_id = "missing_texture";
                }
            };
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
                    shoot_mouse_position = engine.window.MapPixelToCoords(event.position, *cam_comp);
                    if (!physics::GetCollider(entity).TestPoint(shoot_mouse_position))
                        continue;

                    shoot_line.GetComponent<Position>().position = shoot_mouse_position;
                    for (auto& vertex : shoot_line.GetComponent<PolygonShape>().vertices)
                        vertex.color = Color::White();
                    
                    shoot_particle_position = entity.GetIfHasComponentElseDefault<Position>().position;
                    shoot_particle = entity.id;

                    engine.window.window_io.SetMouseCursor(engine.assets.cursors.at("grab"));

                    return;
                }
            }
            else if (event.button == event::MouseButton::Button::Right)
            {
                for (auto [entity, fsm] : engine
                    .Select<CircleShape, StateMachines>()
                    .EntitiesAndOnlyComponents<StateMachines>())
                {
                    fsm::ManualTransition(fsm.state_machines.at(1), 1, entity, engine);
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
                    vertex.color = Color::Transparent();

                // engine.GetEntity(shoot_particle).GetComponent<EntityInfo>().enabled = false;

                shoot_particle = -1;

                engine.assets.sounds.at("shoot").SetVolume(std::clamp(impulse.Magnitude() * 10.f, 0.f, 100.f));
                engine.assets.sounds.at("shoot").Play();

                engine.window.window_io.SetMouseCursor(engine.assets.cursors.at("normal"));
            }
            else if (event.button == event::MouseButton::Button::Right)
            {
                for (auto [entity, fsm] : engine
                    .Select<CircleShape, StateMachines>()
                    .EntitiesAndOnlyComponents<StateMachines>())
                {
                    fsm::ManualTransition(fsm.state_machines.at(1), 0, entity, engine);
                }
            }
        };
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
}

int main()
{
    Engine engine;
    engine.window.settings.fps = 120;

    engine.assets.AddSound("shoot", "shoot", "shoot.wav");

    engine.assets.LoadCursor("normal", io::Cursor::Type::Arrow);
    engine.assets.LoadCursor("grab", io::Cursor::Type::Hand);

    physics::Enable(engine); 
    physics::SetGravity(Vec2f(0, 9.8));

    fsm::Enable(engine);

    GameLoopFunctions glf;
    glf.start           = ParticleSim::Start;
    glf.update          = ParticleSim::Update;
    glf.fixed_update    = ParticleSim::FixedUpdate;

    engine.AddScene("particle_sim", glf);

    engine.StartScene("particle_sim", Window::Settings{ .size = Vec2u(1280, 720), .title = "Particle Simulation" });

    return 0;
}