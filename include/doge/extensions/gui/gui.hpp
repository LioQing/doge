#pragma once

#include <doge/utils/aliases.hpp>
#include <doge/extensions/gui/GUIComponent.hpp>
#include <doge/extensions/gui/GUIElement.hpp>
#include <doge/core/Engine.hpp>
#include <unordered_map>
#include <vector>
#include <memory>
#include <doge/utils/aliases.hpp>

namespace doge
{
    struct gui
    {
        using Element = GUIElement;
        using Component = GUIComponent;

        gui(const gui&) = delete;
        static void Enable(Engine& engine);
        static void Disable(Engine& engine);

        static Entity AddCamera(Engine& engine, const std::string& id, int camera_layer = 32);

        template <std::convertible_to<std::string>... T>
        static Entity AddCamera(Engine& engine, const std::string& id, int camera_layer, T&&... scene_ids)
        {
            Entity cam = engine.AddEntity(scene_ids...);
            cam.AddComponent<Camera>();
            cam.AddComponent(Layer::Create(camera_layer));

            camera_entities.emplace(id, cam);

            return cam;
        }

        template <typename E>
        requires std::derived_from<std::remove_reference_t<E>, Element>
        static void AddElement(Engine& engine, E&& element)
        {
            auto entity = engine.AddEntity();
            auto& comp = entity.AddComponent(Component::Create(std::forward<E>(element)));

            if (element.GetID() == "")
                idless_elements.emplace_back(comp);
            else
                elements.emplace(element.GetID(), comp);
        }

        static void RemoveElement(const std::string& id);
        static void ClearElements();

    private:

        static std::unordered_map<std::string, Entity> camera_entities;
        static std::unordered_map<std::string, std::reference_wrapper<Component>> elements;
        static std::vector<std::reference_wrapper<Component>> idless_elements;

        static void Start(Engine& engine);
        static void Update(Engine& engine, DeltaTime dt);
        static void Finish(Engine& engine);
    };
}