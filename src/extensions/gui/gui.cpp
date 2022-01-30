#include <doge/extensions/gui/GUI.hpp>

#include <doge/core/Engine.hpp>
#include <doge/extensions/nine_slice.hpp>

namespace doge::gui
{
    GUI::GUI(Engine& engine) : engine(engine), nine_slice(engine)
    {
        GameLoopFunctions glf;
        glf.start = [&](Engine& engine){ Start(engine); };
        glf.update = [&](Engine& engine, DeltaTime dt){ Update(engine, dt); };
        glf.fixed_update = [&](Engine& engine, DeltaTime dt){ FixedUpdate(engine, dt); };
        glf.finish = [&](Engine& engine){ Finish(engine); };
        
        engine.scenes.extensions.emplace("doge_gui", glf);

        engine.assets.LoadTexture("doge_gui_button", "gui/button.png");
        nine_slice.LoadTexture("doge_gui_button", "gui/button.png", Recti(8, 8, 8, 8));
    }

    GUI::~GUI()
    {
        engine.assets.EraseTexture("doge_gui_button");
        nine_slice.EraseTexture("doge_gui_button");

        engine.scenes.extensions.erase("doge_gui");
    }

    void GUI::AddCamera(const std::string& id, std::int32_t render_order, std::int32_t start_layer, std::int32_t end_layer, bool destroy_on_finish)
    {
        auto [itr, success] = cameras.emplace(id, engine.AddEntity(destroy_on_finish));

        if (!success)
            throw std::invalid_argument("Failed to add camera to gui");

        itr->second.AddComponent(Camera{ .render_order = render_order });

        std::set<std::int32_t> layers;
        for (std::int32_t i = start_layer; i < end_layer; ++i)
            layers.emplace(i);

        itr->second.AddComponent<Layer>(layers);
    }

    void GUI::RemoveCamera(const std::string& id)
    {
        RemoveElements(id);
        cameras.erase(id);
    }

    doge::Component<Camera>& GUI::GetCameraComponent(const std::string& id)
    {
        return cameras.at(id).GetComponent<Camera>();
    }

    Entity GUI::GetCameraEntity(const std::string& id)
    {
        return cameras.at(id);
    }

    std::int32_t GUI::GetCameraLayer(const std::string& id)
    {
        return (*GetCameraEntity(id).GetComponent<Layer>().layers.begin() + *GetCameraEntity(id).GetComponent<Layer>().layers.rbegin()) / 2;
    }

    const std::set<std::int32_t>& GUI::GetCameraLayers(const std::string& id)
    {
        return GetCameraEntity(id).GetComponent<Layer>().layers;
    }

    bool GUI::HasCamera(const std::string& id)
    {
        return cameras.find(id) != cameras.end();
    }

    void GUI::RemoveElement(const std::string& id)
    {
        engine.DestroyEntity(elements.at(id));
        elements.erase(id);
    }

    void GUI::RemoveElements(const std::string& camera_id)
    {
        for (auto& [id, element] : elements)
        {
            if (element.GetComponent<Component>().element->GetCameraID() == camera_id)
                engine.DestroyEntity(element);
        }

        for (auto& element : idless_elements)
        {
            if (element.GetComponent<Component>().element->GetCameraID() == camera_id)
                engine.DestroyEntity(element);
        }
    }

    Element& GUI::GetElement(const std::string& id)
    {
        return *GetElementComponent(id).element;
    }

    doge::Component<Component>& GUI::GetElementComponent(const std::string& id)
    {
        return elements.at(id).GetComponent<Component>();
    }

    Entity GUI::GetElementEntity(const std::string& id)
    {
        return elements.at(id);
    }

    bool GUI::HasElement(const std::string& id)
    {
        return elements.find(id) != elements.end();
    }

    Engine& GUI::GetEngine()
    {
        return engine;
    }

    doge::nine_slice::NineSlice& GUI::GetNineSlice()
    {
        return nine_slice;
    }

    void GUI::Start(Engine& engine)
    {

    }

    void GUI::Update(Engine& engine, DeltaTime dt)
    {
        for (auto& [id, element] : elements)
        {
            element.GetComponent<Component>().element->Update(dt);
        }

        for (auto& element : idless_elements)
        {
            element.GetComponent<Component>().element->Update(dt);
        }
    }

    void GUI::FixedUpdate(Engine& engine, DeltaTime dt)
    {
        for (auto& [id, element] : elements)
        {
            element.GetComponent<Component>().element->FixedUpdate(dt);
        }

        for (auto& element : idless_elements)
        {
            element.GetComponent<Component>().element->FixedUpdate(dt);
        }
    }

    void GUI::Finish(Engine& engine)
    {
        
    }
}