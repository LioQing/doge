#pragma once

#include <memory>
#include <concepts>
#include <doge/extensions/gui/GUIElement.hpp>

namespace doge
{
    struct GUIElementComponent
    {
        std::shared_ptr<GUIElement> element;

        template <typename E>
        requires std::derived_from<std::remove_reference_t<E>, GUIElement>
        static GUIElementComponent Create(E&& element)
        {
            GUIElementComponent comp;
            comp.element = std::make_shared<std::remove_reference_t<E>>(std::forward<E>(element));
            return comp;
        }
    };
}