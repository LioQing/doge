#pragma once

#include <doge/utils/aliases.hpp>
#include <doge/extensions/gui/GUIComponent.hpp>
#include <doge/extensions/gui/GUIElement.hpp>
#include <doge/core/Engine.hpp>
#include <map>
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

    private:

        static std::map<std::string, std::reference_wrapper<Component>> elements;
        static std::vector<std::reference_wrapper<Component>> idless_elements;

        static void Start(Engine& engine);
        static void Update(Engine& engine, DeltaTime dt);
        static void Finish(Engine& engine);
    };
}