#pragma once

#include <doge/extensions/gui/Window.hpp>
#include <bitset>

namespace doge::gui
{
    struct Text;
    struct Draggable;
    struct Resizable;

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

        static const Rectf DefaultResizeThickness;
        static const Rectf DefaultBorderThickness;

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

        std::string GetResizableElementID() const;
        gui::Resizable& GetResizable() const;

        void SetBorderThickness(const Rectf& border_thickness);

    protected:

        virtual void OnSizeUpdated() override;
        virtual void OnPositionUpdated() override;
        virtual void OnOriginUpdated() override;
        
        virtual void UpdateTitleBarOrigin();

        virtual void UpdateDraggableOrigin();
        virtual void UpdateDraggableSize();

        virtual void UpdateResizableSize();
        virtual void UpdateResizableOrigin();

    private:

        std::bitset<Trait::Count> trait_enabled;

        Vec2f drag_start_pos;
        Vec2f resize_start_pos;
        bool resize_guard = false;
    };
}