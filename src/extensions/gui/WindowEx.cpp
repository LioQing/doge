#include <doge/extensions/gui/WindowEx.hpp>

namespace doge::gui
{
    WindowEx::~WindowEx()
    {
    }

    void WindowEx::Initialize()
    {
        Window::Initialize();

        SetContainerBorderThickness(Rectf(16, 16, 16, 16));
    }

    void WindowEx::SetTitleBar(bool enabled)
    {
        trait_enabled.set(Trait::TitleBar, enabled);
    }

    bool WindowEx::HasTitleBar() const
    {
        return trait_enabled.test(Trait::TitleBar);
    }

    void WindowEx::SetDraggable(bool enabled)
    {
        trait_enabled.set(Trait::Draggable, enabled);
    }

    bool WindowEx::IsDraggable() const
    {
        return trait_enabled.test(Trait::Draggable);        
    }

    void WindowEx::SetResizable(bool enabled)
    {
        trait_enabled.set(Trait::Resizable, enabled);
    }

    bool WindowEx::IsResizable() const
    {
        return trait_enabled.test(Trait::Resizable);
    }

    void WindowEx::SetScrollable(bool enabled)
    {
        trait_enabled.set(Trait::Scrollable, enabled);
    }

    bool WindowEx::IsScrollable() const
    {
        return trait_enabled.test(Trait::Scrollable);
    }
}