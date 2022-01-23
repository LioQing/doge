#pragma once

#include <doge/utils/aliases.hpp>
#include <doge/extensions/gui/GUIComponent.hpp>
#include <doge/extensions/gui/GUIElement.hpp>
#include <doge/core/Engine.hpp>
#include <unordered_map>
#include <unordered_set>
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

        static void AddCamera(Engine& engine, const std::string& id, int layer = 32, bool destroy_on_finish = true);

        static void RemoveCamera(Engine& engine, const std::string& id);

        template <typename E>
        requires std::derived_from<std::remove_reference_t<E>, Element>
        static void AddElement(Engine& engine, E&& element)
        {
            if (cameras.find(element.GetOwnerCamera()) == cameras.end())
                throw std::invalid_argument("OwnerCamera property of GUIElement (gui::Element) is not found in gui::cameras");

            auto entity = engine.AddEntity();
            Element& element = *entity.AddComponent(Component::Create(std::forward<E>(element))).element;

            if (element.GetID() == "")
                idless_elements.emplace(entity);
            else
                elements.emplace(element.GetID(), entity);
            
            element.Initialize(engine);
        }

        static void RemoveElement(Engine& engine, const std::string& id);
        static void RemoveElements(Engine& engine, const std::string& camera_id);

    private:

        static std::unordered_map<std::string, Entity> cameras;
        static std::unordered_map<std::string, Entity> elements;
        static std::set<Entity> idless_elements;

        static void Update(Engine& engine, DeltaTime dt);
        static void FixedUpdate(Engine& engine, DeltaTime dt);
    };
}