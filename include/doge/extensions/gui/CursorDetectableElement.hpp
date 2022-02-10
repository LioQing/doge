#pragma once

#include <doge/extensions/gui/Element.hpp>
#include <doge/utils/lev.hpp>
#include <doge/utils/Vec2.hpp>
#include <doge/core/io/Mouse.hpp>

namespace doge::gui
{
    struct GUI;

    struct CursorDetectableElement : Element
    {
        Event<Vec2f, io::Mouse::Button> on_pressed;
        Event<Vec2f, io::Mouse::Button> on_released;
        Event<Vec2f> on_cursor_moved;
        Event<Vec2f> on_cursor_entered;
        Event<Vec2f> on_cursor_left;

        virtual ~CursorDetectableElement() = 0 {};

    protected:

        virtual void OnPressed(const Vec2f& position, io::Mouse::Button button) {};
        virtual void OnReleased(const Vec2f& position, io::Mouse::Button button) {};
        virtual void OnCursorMoved(const Vec2f& position) {};
        virtual void OnCursorEntered(const Vec2f& position) {};
        virtual void OnCursorLeft(const Vec2f& position) {};

    private:

        friend struct GUI;
    };
}