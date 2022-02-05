#pragma once

#include <doge/extensions/gui/Element.hpp>
#include <doge/utils/Vec2.hpp>

namespace doge::gui
{
    struct Draggable : Element
    {
        Event<Vec2f> on_drag_began;
        Event<Vec2f> on_drag_ended;
        Event<Vec2f> on_dragged;

        virtual ~Draggable();

        virtual void Initialize() override;

        bool IsDragging() const;

    private:

        bool is_dragging = false;
    };
}