#pragma once

#include <doge/extensions/gui/CursorDetectableElement.hpp>
#include <doge/utils/Vec2.hpp>

namespace doge::gui
{
    struct Draggable : CursorDetectableElement
    {
        Event<Vec2f> on_drag_began;
        Event<Vec2f> on_drag_ended;
        Event<Vec2f> on_dragged;
        Event<Vec2f> on_dragged_diff;

        virtual ~Draggable();

        virtual void Initialize() override;

        bool IsDragging() const;

    protected:

        virtual void OnPressed(const Vec2f& position, io::Mouse::Button button) override;
        virtual void OnReleased(const Vec2f& position, io::Mouse::Button button) override;
        virtual void OnCursorMoved(const Vec2f& position) override;

    private:

        Vec2f CoordsChecked(const Vec2f& coords_) const;

        bool is_dragging = false;

        Vec2f mouse_start;
    };
}