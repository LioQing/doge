#include <doge/extensions/gui/WindowEx.hpp>

#include <doge/extensions/gui/GUI.hpp>
#include <doge/extensions/gui/Text.hpp>

namespace doge::gui
{
    WindowEx::~WindowEx()
    {
    }

    void WindowEx::Initialize()
    {
        // titlebar
        auto& title_bar = GetGUI().AddElement<Text>(GetTitleBarElementID(), GetCameraID());
        title_bar.GetEntity().SetParent(GetEntity());
        title_bar.SetLocalLayer(3);
        title_bar.SetCursorDetectable(false);
        title_bar.SetAlign(Align::Top | Align::Left);
        title_bar.SetFont("arial");
        title_bar.SetFontSize(12);
        title_bar.SetString(U"WindowEx Title");
        title_bar.SetAppearance(doge::Text::Appearance{ .fill_color = Color::Black });

        Window::Initialize();

        SetBorderThickness(Rectf(16, 16, 16, 16));
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

    std::string WindowEx::GetTitleBarElementID() const
    {
        return "doge_gui_windowex_" + GetID() + "_title_bar";
    }

    Text& WindowEx::GetTitleBar() const
    {
        return static_cast<Text&>(GetGUI().GetElement(GetTitleBarElementID()));
    }

    void WindowEx::OnSizeUpdated()
    {
        Window::OnSizeUpdated();

        GetTitleBar().SetOrigin(GetActualOrigin() - GetBorderThickness().left * Vec2f::i);
    }
    
    void WindowEx::OnPositionUpdated()
    {
        Window::OnPositionUpdated();
    }
    
    void WindowEx::OnOriginUpdated()
    {
        Window::OnOriginUpdated();

        GetTitleBar().SetOrigin(GetActualOrigin() - GetBorderThickness().left * Vec2f::i);
    }
}