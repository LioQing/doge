#include <numbers>
#include <cmath>

#include <doge/doge.hpp>

struct CircleAngle
{
    float angle = 0.f;
};

void Start(doge::Engine& engine)
{
    // Add a camera
    engine.AddCamera();

    // Add an entity (note doge::Entity is better passed by value)
    doge::Entity entity = engine.AddEntity();

    // Add a doge::Position component to the entity with given arguments
    entity.AddComponent<doge::Position>(300, 0);

    // Add a CircleAngle class as component to the entity
    entity.AddComponent<CircleAngle>(0);

    // Assign the added component to local variable, note the returned type is doge::Component<T>&
    doge::Component<doge::CircleShape>& circle_shape = entity.AddComponent(doge::CircleShape
        {
            .radius = 20,
            .origin = { 20, 20 },
            .texture_id = "missing_texture",
            .texture_rectangle = { 0, 0, 32, 32 },
        });

    // Add a listener to the on_mouse_button_pressed event
    engine.events.on_mouse_button_pressed.AddListener("ChangeTexture",
        [&](const doge::event::MouseButton& event)
        {
            if (circle_shape.texture_id == "missing_texture")
            {
                circle_shape.texture_id = "";
                circle_shape.color = doge::Color::Green;
            }
            else
            {
                circle_shape.texture_id = "missing_texture";
                circle_shape.color = doge::Color::White;
            }
        });
}

void Update(doge::Engine& engine, doge::DeltaTime dt)
{
    // Cycle through all entities with CircleAngle and doge::Position components
    // Components() will return components as references in a tuple
    for (auto [circle_angle, position] : engine.Select<CircleAngle, doge::Position>().Components())
    {
        circle_angle.angle = std::fmod(circle_angle.angle + dt * 0.002, 2 * std::numbers::pi_v<float>);
        position.position = doge::Vec2f(300, 0).Rotated(circle_angle.angle);
    }
}

void Finish(doge::Engine& engine)
{
    // Remember to remove event listeners if you change scenes, it doesn't matter if you are not
    engine.events.on_mouse_button_pressed.RemoveListener("ChangeTexture");
}

int main()
{
    // Construct the engine
    doge::Engine engine;

    // Set the window settings
    engine.window.settings.size = doge::Vec2u(1280, 720);
    engine.window.settings.fps = 60;
    engine.window.settings.title = "DOGE!";

    // Construct a set of game loop functions
    doge::GameLoopFunctions glf
    {
        .start = Start,
        .update = Update,
        .finish = Finish,
    };

    // Add the game loop functions as a scene
    engine.AddScene("MyScene", glf);

    // Start the scene
    engine.StartScene("MyScene");

    return 0;
}