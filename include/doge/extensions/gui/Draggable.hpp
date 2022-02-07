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
        Event<Vec2f> on_dragged_diff;

        virtual ~Draggable();

        virtual void Initialize() override;


        bool IsDragging() const;

    private:

        Vec2f MapPixelToCoordsChecked(const Vec2i& pixel) const;

        bool is_dragging = false;

        Vec2f mouse_start;
    };
}