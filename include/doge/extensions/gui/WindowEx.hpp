#pragma once

#include <doge/extensions/gui/Window.hpp>
#include <bitset>

namespace doge::gui
{
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

    private:

        std::bitset<Trait::Count> trait_enabled;
    };
}