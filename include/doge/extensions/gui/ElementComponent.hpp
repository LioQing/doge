#pragma once

#include <memory>
#include <concepts>
#include <doge/extensions/gui/Element.hpp>

namespace doge::gui
{
    struct ElementComponent
    {
        std::shared_ptr<Element> element;

        template <typename E>
        requires std::derived_from<std::remove_reference_t<E>, Element>
        static ElementComponent Create(E&& element)
        {
            ElementComponent comp;
            comp.element = std::make_shared<std::remove_reference_t<E>>(std::forward<E>(element));
            return comp;
        }
    };
}