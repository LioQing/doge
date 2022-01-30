#pragma once

#include <memory>
#include <concepts>
#include <doge/extensions/gui/Element.hpp>

namespace doge::gui
{
    struct Component
    {
        std::shared_ptr<Element> element;

        template <typename E>
        requires std::derived_from<std::remove_reference_t<E>, Element>
        static Component Create(E&& element)
        {
            Component comp;
            comp.element = std::make_shared<std::remove_reference_t<E>>(std::forward<E>(element));
            return comp;
        }
    };
}