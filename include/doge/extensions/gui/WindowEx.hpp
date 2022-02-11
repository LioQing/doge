#pragma once

#include <doge/extensions/gui/Window.hpp>
#include <bitset>

namespace doge::gui
{
    struct Text;
    struct Draggable;
    struct Resizable;
    struct Button;
    struct CursorDetectableElement;

    struct WindowEx : Window
    {
        enum Trait
        {
            TitleBar = 0,
            Draggable,
            Resizable,
            CloseButton,
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

        void SetCloseButton(bool enabled);
        bool HasCloseButton() const;

        void SetScrollable(bool enabled);
        bool IsScrollable() const;

        std::string GetTitleBarElementID() const;
        Text& GetTitleBar() const;

        std::string GetDraggableElementID() const;
        gui::Draggable& GetDraggable() const;

        std::string GetResizableElementID() const;
        gui::Resizable& GetResizable() const;

        std::string GetCloseButtonElementID() const;
        Button& GetCloseButton() const;

        std::string GetCursorDetectableElementID() const;
        CursorDetectableElement& GetCursorDetectableElement() const;

        void SetBorderThickness(const Rectf& border_thickness);

    protected:

        virtual void OnLayerUpdated() override;
        virtual void OnSizeUpdated() override;
        virtual void OnPositionUpdated() override;
        virtual void OnOriginUpdated() override;
        
        virtual void UpdateTitleBarLayer();
        virtual void UpdateTitleBarOrigin();

        virtual void UpdateDraggableLayer();
        virtual void UpdateDraggableOrigin();
        virtual void UpdateDraggableSize();

        virtual void UpdateResizableLayer();
        virtual void UpdateResizableSize();
        virtual void UpdateResizableOrigin();

        virtual void UpdateCloseButtonLayer();
        virtual void UpdateCloseButtonOrigin();

    private:

        std::bitset<Trait::Count> trait_enabled;

        Vec2f drag_start_pos;
        Vec2f resize_start_pos;
        bool resize_guard = false;
    };
}