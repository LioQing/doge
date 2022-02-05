#include <doge/extensions/gui/WindowEx.hpp>

#include <doge/extensions/gui/GUI.hpp>
#include <doge/extensions/gui/Text.hpp>
#include <doge/extensions/gui/Draggable.hpp>

namespace doge::gui
{
    WindowEx::~WindowEx()
    {
    }

    void WindowEx::Initialize()
    {
        Window::Initialize();

        SetBorderThickness(Rectf(16, 16, 16, 16));
    }

    void WindowEx::SetTitleBar(bool enabled)
    {
        if (HasTitleBar() && !enabled)
        {
            GetGUI().RemoveElement(GetTitleBarElementID());
        }
        else if (!HasTitleBar() && enabled)
        {
            auto& title_bar = GetGUI().AddElement<Text>(GetTitleBarElementID(), GetCameraID());
            title_bar.GetEntity().SetParent(GetEntity());
            title_bar.SetLocalLayer(3);
            title_bar.SetCursorDetectable(false);
            title_bar.SetAlign(Align::Top | Align::Left);
            title_bar.SetFont("arial");
            title_bar.SetFontSize(12);
            title_bar.SetString(U"WindowEx Title");
            title_bar.SetAppearance(doge::Text::Appearance{ .fill_color = Color::Black });
            title_bar.SetOrigin(GetActualOrigin() - GetBorderThickness().left * Vec2f::i);
        }

        trait_enabled.set(Trait::TitleBar, enabled);
    }

    bool WindowEx::HasTitleBar() const
    {
        return trait_enabled.test(Trait::TitleBar);
    }

    void WindowEx::SetDraggable(bool enabled)
    {
        if (IsDraggable() && !enabled)
        {
            GetGUI().RemoveElement(GetDraggableElementID());
        }
        else if (!IsDraggable() && enabled)
        {
            auto& draggable = GetGUI().AddElement<gui::Draggable>(GetDraggableElementID(), GetCameraID());
            draggable.GetEntity().SetParent(GetEntity());
            draggable.SetLocalLayer(3);
            draggable.SetSize(Vec2f(GetSize().x, GetBorderThickness().top - resize_thickness));
            draggable.SetAlign(Align::Top | Align::Left);
            draggable.SetOrigin(GetActualOrigin() - resize_thickness * Vec2f::j);
            draggable.on_drag_began += [&](const Vec2f& pos){ drag_start_event = pos; drag_start_pos = GetPosition(); };
            draggable.on_dragged += [&](const Vec2f& pos){ SetPosition(drag_start_pos + pos - drag_start_event); };
        }

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
        if (!HasTitleBar())
            throw std::invalid_argument("Title bar is not enabled");

        return static_cast<Text&>(GetGUI().GetElement(GetTitleBarElementID()));
    }

    std::string WindowEx::GetDraggableElementID() const
    {
        return "window_gui_windowex_" + GetID() + "_draggable";
    }

    gui::Draggable& WindowEx::GetDraggable() const
    {
        if (!IsDraggable())
            throw std::invalid_argument("Draggable is not enabled");

        return static_cast<gui::Draggable&>(GetGUI().GetElement(GetDraggableElementID()));
    }

    void WindowEx::SetBorderThickness(const Rectf& border_thickness)
    {
        Window::SetBorderThickness(border_thickness);

        if (IsDraggable())
        {
            GetDraggable().SetSize(Vec2f(GetSize().x, GetBorderThickness().top - resize_thickness));
        }
    }

    void WindowEx::OnSizeUpdated()
    {
        Window::OnSizeUpdated();

        if (HasTitleBar())
        {
            GetTitleBar().SetOrigin(GetActualOrigin() - GetBorderThickness().left * Vec2f::i);
        }
        
        if (IsDraggable())
        {
            GetDraggable().SetOrigin(GetActualOrigin() - resize_thickness * Vec2f::j);
            GetDraggable().SetSize(Vec2f(GetSize().x, GetBorderThickness().top - resize_thickness));
        }
    }
    
    void WindowEx::OnPositionUpdated()
    {
        Window::OnPositionUpdated();
    }
    
    void WindowEx::OnOriginUpdated()
    {
        Window::OnOriginUpdated();

        if (HasTitleBar())
        {
            GetTitleBar().SetOrigin(GetActualOrigin() - GetBorderThickness().left * Vec2f::i);
        }
        
        if (IsDraggable())
        {
            GetDraggable().SetOrigin(GetActualOrigin() - resize_thickness * Vec2f::j);
        }
    }
}