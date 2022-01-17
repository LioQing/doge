#pragma once

#include <doge/extensions/gui/GUIElement.hpp>
#include <doge/utils/Vec2.hpp>
#include <doge/utils/lev.hpp>

namespace doge
{
    struct Engine;

    struct GUIInput : GUIElement
    {
        virtual ~GUIInput() = 0;

        Event<Engine&> on_value_changed;
    };
}