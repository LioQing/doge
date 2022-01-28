#include <doge/extensions/gui/gui.hpp>

#include <doge/core/Engine.hpp>
#include <doge/extensions/nine_slice.hpp>

namespace doge
{
    std::unordered_map<std::string, Entity> gui::cameras;
    std::unordered_map<std::string, Entity> gui::elements;
    std::set<Entity> gui::idless_elements;
    Engine* gui::engine;

    void gui::Enable(Engine& engine)
    {
        gui::engine = &engine;

        GameLoopFunctions glf;
        glf.start = Start;
        glf.update = Update;
        glf.fixed_update = FixedUpdate;
        glf.finish = Finish;
        
        engine.scenes.extensions.emplace("doge_gui", glf);

        engine.events.on_window_opened.AddListener(
            "doge_gui_on_window_opened", 
            [&]()
            {
                engine.assets.LoadTexture("doge_gui_button", "gui/button.png");
                nine_slice::LoadTexture(engine.assets, "doge_gui_button", "gui/button.png", Recti(8, 8, 8, 8));
            }
        );
    }

    void gui::Disable()
    {
        engine->events.on_window_opened.RemoveListener("doge_gui_on_window_opened");

        engine->scenes.extensions.erase("doge_gui");
    }

    void gui::AddCamera(const std::string& id, std::int32_t render_order, std::int32_t start_layer, std::int32_t end_layer, bool destroy_on_finish)
    {
        auto [itr, success] = cameras.emplace(id, engine->AddEntity(destroy_on_finish));

        if (!success)
            throw std::invalid_argument("Failed to add camera to gui");

        itr->second.AddComponent(Camera{ .render_order = render_order });

        std::set<std::int32_t> layers;
        for (std::int32_t i = start_layer; i < end_layer; ++i)
            layers.emplace(i);

        itr->second.AddComponent<Layer>(layers);
    }

    void gui::RemoveCamera(const std::string& id)
    {
        RemoveElements(id);
        cameras.erase(id);
    }

    doge::Component<Camera>& gui::GetCameraComponent(const std::string& id)
    {
        return cameras.at(id).GetComponent<Camera>();
    }

    Entity gui::GetCameraEntity(const std::string& id)
    {
        return cameras.at(id);
    }

    std::int32_t gui::GetCameraLayer(const std::string& id)
    {
        return (*GetCameraEntity(id).GetComponent<Layer>().layers.begin() + *GetCameraEntity(id).GetComponent<Layer>().layers.rbegin()) / 2;
    }

    const std::set<std::int32_t>& gui::GetCameraLayers(const std::string& id)
    {
        return GetCameraEntity(id).GetComponent<Layer>().layers;
    }

    bool gui::HasCamera(const std::string& id)
    {
        return cameras.find(id) != cameras.end();
    }

    void gui::RemoveElement(const std::string& id)
    {
        engine->DestroyEntity(elements.at(id));
        elements.erase(id);
    }

    void gui::RemoveElements(const std::string& camera_id)
    {
        for (auto& [id, element] : elements)
        {
            if (element.GetComponent<GUIElementComponent>().element->GetCameraID() == camera_id)
                engine->DestroyEntity(element);
        }

        for (auto& element : idless_elements)
        {
            if (element.GetComponent<GUIElementComponent>().element->GetCameraID() == camera_id)
                engine->DestroyEntity(element);
        }
    }

    GUIElement& gui::GetElement(const std::string& id)
    {
        return *GetElementComponent(id).element;
    }

    doge::Component<GUIElementComponent>& gui::GetElementComponent(const std::string& id)
    {
        return elements.at(id).GetComponent<GUIElementComponent>();
    }

    Entity gui::GetElementEntity(const std::string& id)
    {
        return elements.at(id);
    }

    bool gui::HasElement(const std::string& id)
    {
        return elements.find(id) != elements.end();
    }

    Engine& gui::GetEngine()
    {
        return *engine;
    }

    void gui::Start(Engine& engine)
    {

    }

    void gui::Update(Engine& engine, DeltaTime dt)
    {
        for (auto& [id, element] : elements)
        {
            element.GetComponent<GUIElementComponent>().element->Update(dt);
        }

        for (auto& element : idless_elements)
        {
            element.GetComponent<GUIElementComponent>().element->Update(dt);
        }
    }

    void gui::FixedUpdate(Engine& engine, DeltaTime dt)
    {
        for (auto& [id, element] : elements)
        {
            element.GetComponent<GUIElementComponent>().element->FixedUpdate(dt);
        }

        for (auto& element : idless_elements)
        {
            element.GetComponent<GUIElementComponent>().element->FixedUpdate(dt);
        }
    }

    void gui::Finish(Engine& engine)
    {
        
    }
}