#pragma once

#include <doge/utils/aliases.hpp>
#include <doge/extensions/gui/GUIElementComponent.hpp>
#include <doge/extensions/gui/GUIElement.hpp>
#include <doge/core/Engine.hpp>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <doge/utils/aliases.hpp>

namespace doge
{
    struct Camera;

    struct gui
    {
        using Element = GUIElement;

        gui(const gui&) = delete;
        static void Enable(Engine& engine);
        static void Disable(Engine& engine);

        static void AddCamera(Engine& engine, const std::string& id, std::int32_t render_order = 32, std::int32_t layer = 32, bool destroy_on_finish = true);

        static void RemoveCamera(Engine& engine, const std::string& id);

        static doge::Component<Camera>& GetCameraComponent(const std::string& id);
        static Entity GetCameraEntity(const std::string& id);
        static std::int32_t GetCameraLayer(const std::string& id);

        static bool HasCamera(const std::string& id);

        template <typename E>
        requires std::derived_from<std::remove_reference_t<E>, Element>
        static void AddElement(Engine& engine, const std::string& id, E&& element)
        {
            auto cam_itr = cameras.find(element.GetCameraID());
            if (cam_itr == cameras.end())
                throw std::invalid_argument("Camera property of GUIElement (gui::Element) is not found in gui::cameras");

            auto entity = engine.AddEntity();
            entity.SetParent(cam_itr->second);

            entity.AddComponent(Layer::Create(*cam_itr->second.GetComponent<Layer>().layers.begin()));

            auto& comp = entity.AddComponent(GUIElementComponent::Create(std::forward<E>(element)));
            comp.element->id = id;

            if (comp.element->GetID() == "")
                idless_elements.emplace(entity);
            else
                elements.emplace(comp.element->GetID(), entity);
            
            comp.element->Initialize(engine);
        }

        static void RemoveElement(Engine& engine, const std::string& id);
        static void RemoveElements(Engine& engine, const std::string& camera_id);

        static Element& GetElement(const std::string& id);
        static doge::Component<GUIElementComponent>& GetElementComponent(const std::string& id);
        static Entity GetElementEntity(const std::string& id);

        static bool HasElement(const std::string& id);

    private:

        static std::unordered_map<std::string, Entity> cameras;
        static std::unordered_map<std::string, Entity> elements;
        static std::set<Entity> idless_elements;

        static void Start(Engine& engine);
        static void Update(Engine& engine, DeltaTime dt);
        static void FixedUpdate(Engine& engine, DeltaTime dt);
        static void Finish(Engine& engine);
    };
}