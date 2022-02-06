#pragma once

#include <doge/utils/Rect.hpp>
#include <doge/extensions/gui/Element.hpp>
#include <bitset>

namespace doge::gui
{
    struct Draggable;

    struct Resizable : Element
    {
        enum Border : std::int8_t
        {
            TopLeft,
            Top,
            TopRight,
            Right,
            BottomRight,
            Bottom,
            BottomLeft,
            Left,

            Count
        };

        Event<> on_resize_began;
        Event<> on_resize_ended;
        Event<Vec2f> on_resized;

        virtual ~Resizable();

        virtual void Initialize() override;

        void SetThickness(const Rectf& thickness);
        const Rectf& GetThickness() const;

        void SetLocalLayer(std::int32_t layer);
        void SetLayer(std::int32_t layer);
        std::int32_t GetLocalLayer() const;
        std::int32_t GetLayer() const;

        void SetCursorDetectable(bool is_cursor_detectable);
        bool IsCursorDetectable() const;

        std::string GetDraggableElementID(std::int8_t border) const;
        Draggable& GetDraggable(std::int8_t border) const;

        Border GetBorderDragged() const;

    protected:

        virtual void OnSizeUpdated() override;
        virtual void OnPositionUpdated() override;
        virtual void OnOriginUpdated() override;

    private:

        Border border_dragged;
        Rectf thickness;
        Vec2f start_pos;
        Vec2f start_size;
    };
}