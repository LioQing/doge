#include <doge/extensions/gui/gui.hpp>

#include <doge/core/Engine.hpp>

namespace doge
{
    std::unordered_map<std::string, Entity> gui::camera_entities;
    std::unordered_map<std::string, std::reference_wrapper<gui::Component>> gui::elements;
    std::vector<std::reference_wrapper<gui::Component>> gui::idless_elements;

    void gui::Enable(Engine& engine)
    {
        GameLoopFunctions glf;
        glf.start = Start;
        glf.update = Update;
        glf.finish = Finish;
        
        engine.scenes.extensions.emplace("doge_gui", glf);
    }

    void gui::Disable(Engine& engine)
    {
        engine.scenes.extensions.erase("doge_gui");
    }

    Entity gui::AddCamera(Engine& engine, const std::string& id, int camera_layer)
    {
        Entity cam = engine.AddCamera();
        cam.AddComponent(Layer::Create(camera_layer));

        camera_entities.emplace(id, cam);

        return cam;
    }

    void gui::RemoveElement(const std::string& id)
    {
        elements.erase(id);
    }

    void gui::ClearElements()
    {
        
    }

    void gui::Start(Engine& engine)
    {

    }

    void gui::Update(Engine& engine, DeltaTime dt)
    {

    }

    void gui::Finish(Engine& engine)
    {

    }
}