#pragma once

#include <doge/utils/aliases.hpp>
#include <doge/extensions/gui/GUIElementComponent.hpp>
#include <doge/extensions/gui/GUIElement.hpp>
#include <doge/extensions/nine_slice.hpp>
#include <doge/core/Engine.hpp>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <doge/utils/aliases.hpp>

namespace doge
{
    struct Camera;

    struct GUI
    {
        using Element = GUIElement;

        GUI(Engine& engine);
        ~GUI();

        void AddCamera(const std::string& id, std::int32_t render_order = 32, std::int32_t start_layer = 32, std::int32_t end_layer = 36, bool destroy_on_finish = true);

        void RemoveCamera(const std::string& id);

        doge::Component<Camera>& GetCameraComponent(const std::string& id);
        Entity GetCameraEntity(const std::string& id);
        std::int32_t GetCameraLayer(const std::string& id);
        const std::set<std::int32_t>& GetCameraLayers(const std::string& id);

        bool HasCamera(const std::string& id);

        template <typename E>
        requires std::derived_from<std::remove_reference_t<E>, Element>
        E& AddElement(const std::string& id, const std::string& cam_id)
        {
            auto cam_itr = cameras.find(cam_id);
            if (cam_itr == cameras.end())
                throw std::invalid_argument("Camera property of GUIElement (GUI::Element) is not found in GUI::cameras");

            auto entity = engine.AddEntity();
            entity.SetParent(cam_itr->second);

            entity.AddComponent(Layer::Create(*cam_itr->second.GetComponent<Layer>().layers.begin()));

            auto& comp = entity.AddComponent(GUIElementComponent::Create(E()));
            comp.element->id = id;
            comp.element->camera = cam_id;
            comp.element->gui = this;

            if (comp.element->GetID() == "")
                idless_elements.emplace(entity);
            else
                elements.emplace(comp.element->GetID(), entity);
            
            comp.element->Initialize();

            return static_cast<E&>(*comp.element);
        }

        void RemoveElement(const std::string& id);
        void RemoveElements(const std::string& camera_id);

        Element& GetElement(const std::string& id);
        doge::Component<GUIElementComponent>& GetElementComponent(const std::string& id);
        Entity GetElementEntity(const std::string& id);

        bool HasElement(const std::string& id);

        Engine& GetEngine();
        NineSlice& GetNineSlice();

    private:

        Engine& engine;
        NineSlice nine_slice;

        std::unordered_map<std::string, Entity> cameras;
        std::unordered_map<std::string, Entity> elements;
        std::set<Entity> idless_elements;

        void Start(Engine& engine);
        void Update(Engine& engine, DeltaTime dt);
        void FixedUpdate(Engine& engine, DeltaTime dt);
        void Finish(Engine& engine);
    };
}