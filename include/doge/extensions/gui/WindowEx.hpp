#pragma once

#include <doge/extensions/gui/Window.hpp>
#include <bitset>

namespace doge::gui
{
    struct Text;
    struct Draggable;

    struct WindowEx : Window
    {
        enum Trait
        {
            TitleBar = 0,
            Draggable,
            Resizable,
            Scrollable,

            Count
        };

        static const std::int32_t resize_thickness = 2;

        virtual ~WindowEx();

        virtual void Initialize() override;

        void SetTitleBar(bool enabled);
        bool HasTitleBar() const;

        void SetDraggable(bool enabled);
        bool IsDraggable() const;

        void SetResizable(bool enabled);
        bool IsResizable() const;

        void SetScrollable(bool enabled);
        bool IsScrollable() const;

        std::string GetTitleBarElementID() const;
        Text& GetTitleBar() const;

        std::string GetDraggableElementID() const;
        gui::Draggable& GetDraggable() const;

        void SetBorderThickness(const Rectf& border_thickness);

    protected:

        virtual void OnSizeUpdated() override;
        virtual void OnPositionUpdated() override;
        virtual void OnOriginUpdated() override;

    private:

        std::bitset<Trait::Count> trait_enabled;

        Vec2f drag_start_pos = Vec2f::Zero;
        Vec2f drag_start_event = Vec2f::Zero;
    };
}