#include <doge/extensions/gui/gui.hpp>

#include <doge/core/Engine.hpp>

namespace doge
{
    std::unordered_map<std::string, Entity> gui::cameras;
    std::unordered_map<std::string, Entity> gui::elements;
    std::set<Entity> gui::idless_elements;

    void gui::Enable(Engine& engine)
    {
        GameLoopFunctions glf;
        glf.update = Update;
        glf.fixed_update = FixedUpdate;
        
        engine.scenes.extensions.emplace("doge_gui", glf);
    }

    void gui::Disable(Engine& engine)
    {
        engine.scenes.extensions.erase("doge_gui");
    }

    void gui::AddCamera(Engine& engine, const std::string& id, int layer, bool destroy_on_finish)
    {
        auto [itr, success] = cameras.emplace(id, engine.AddEntity(destroy_on_finish));

        if (!success)
            throw std::invalid_argument("Failed to add camera to gui");

        itr->second.AddComponent<Camera>();
        itr->second.AddComponent(Layer::Create(layer));
    }

    void gui::RemoveCamera(Engine& engine, const std::string& id)
    {
        RemoveElements(engine, id);
        cameras.erase(id);
    }

    doge::Component<Camera>& gui::GetCamera(const std::string& id)
    {
        return cameras.at(id).GetComponent<Camera>();
    }

    void gui::RemoveElement(Engine& engine, const std::string& id)
    {
        engine.DestroyEntity(elements.at(id));
        elements.erase(id);
    }

    void gui::RemoveElements(Engine& engine, const std::string& camera_id)
    {
        for (auto& [id, element] : elements)
        {
            if (element.GetComponent<GUIComponent>().element->GetCamera() == camera_id)
                engine.DestroyEntity(element);
        }

        for (auto& element : idless_elements)
        {
            if (element.GetComponent<GUIComponent>().element->GetCamera() == camera_id)
                engine.DestroyEntity(element);
        }
    }

    doge::Component<gui::Component>& gui::GetElement(const std::string& id)
    {
        return elements.at(id).GetComponent<GUIComponent>();
    }

    void gui::Update(Engine& engine, DeltaTime dt)
    {
        for (auto& [id, element] : elements)
        {
            element.GetComponent<GUIComponent>().element->Update(engine, dt);
        }

        for (auto& element : idless_elements)
        {
            element.GetComponent<GUIComponent>().element->Update(engine, dt);
        }
    }

    void gui::FixedUpdate(Engine& engine, DeltaTime dt)
    {
        for (auto& [id, element] : elements)
        {
            element.GetComponent<GUIComponent>().element->FixedUpdate(engine, dt);
        }

        for (auto& element : idless_elements)
        {
            element.GetComponent<GUIComponent>().element->FixedUpdate(engine, dt);
        }
    }
}