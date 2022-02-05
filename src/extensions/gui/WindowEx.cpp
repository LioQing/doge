#include <doge/extensions/gui/WindowEx.hpp>

#include <doge/extensions/gui/GUI.hpp>
#include <doge/extensions/gui/Text.hpp>
#include <doge/extensions/gui/Draggable.hpp>
#include <doge/extensions/gui/NSImage.hpp>

namespace doge::gui
{
    const std::int32_t WindowEx::DefaultResizeThickness = 2;
    const Rectf WindowEx::DefaultBorderThickness = Rectf(16, 32, 16, 16);

    WindowEx::~WindowEx()
    {
    }

    void WindowEx::Initialize()
    {
        Window::Initialize();

        SetBorderThickness(DefaultBorderThickness);
        GetImage().SetTextureID("doge_gui_windowex");
        GetImage().SetBorderThickness(Rectf());
    }

    void WindowEx::SetTitleBar(bool enabled)
    {
        auto prev_enabled = trait_enabled.test(Trait::TitleBar);
        trait_enabled.set(Trait::TitleBar, enabled);

        if (prev_enabled && !enabled)
        {
            GetGUI().RemoveElement(GetTitleBarElementID());
        }
        else if (!prev_enabled && enabled)
        {
            auto& title_bar = GetGUI().AddElement<Text>(GetTitleBarElementID(), GetCameraID());
            title_bar.GetEntity().SetParent(GetEntity());
            title_bar.SetLocalLayer(3);
            title_bar.SetCursorDetectable(false);
            title_bar.SetAlign(Align::Center | Align::Left);
            title_bar.SetFontSize(14);
            title_bar.SetString(U"WindowEx Title");
            title_bar.SetAppearance(doge::Text::Appearance{ .fill_color = Color::Black });

            UpdateTitleBarOrigin();
        }
    }

    bool WindowEx::HasTitleBar() const
    {
        return trait_enabled.test(Trait::TitleBar);
    }

    void WindowEx::SetDraggable(bool enabled)
    {
        auto prev_enabled = trait_enabled.test(Trait::Draggable);
        trait_enabled.set(Trait::Draggable, enabled);

        if (prev_enabled && !enabled)
        {
            GetGUI().RemoveElement(GetDraggableElementID());
        }
        else if (!prev_enabled && enabled)
        {
            auto& draggable = GetGUI().AddElement<gui::Draggable>(GetDraggableElementID(), GetCameraID());
            draggable.GetEntity().SetParent(GetEntity());
            draggable.SetLocalLayer(3);
            draggable.SetSize(Vec2f(GetSize().x, GetBorderThickness().top - DefaultResizeThickness));
            draggable.SetAlign(Align::Top | Align::Left);

            draggable.on_drag_began += [&](const Vec2f& pos){ drag_start_event = pos; drag_start_pos = GetPosition(); };
            draggable.on_dragged += [&](const Vec2f& pos){ SetPosition(drag_start_pos + pos - drag_start_event); };

            UpdateDraggableOrigin();
        }
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
            UpdateDraggableSize();
        }
    }

    void WindowEx::OnSizeUpdated()
    {
        Window::OnSizeUpdated();

        if (HasTitleBar())
        {
            UpdateTitleBarOrigin();
        }
        
        if (IsDraggable())
        {
            UpdateDraggableOrigin();
            UpdateDraggableSize();
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
            UpdateTitleBarOrigin();
        }
        
        if (IsDraggable())
        {
            UpdateDraggableOrigin();
        }
    }

    void WindowEx::UpdateTitleBarOrigin()
    {
        GetTitleBar().SetOrigin(GetActualOrigin() - Vec2f(GetBorderThickness().left, GetBorderThickness().top / 2.f));
    }

    void WindowEx::UpdateDraggableOrigin()
    {
        GetDraggable().SetOrigin(GetActualOrigin() - DefaultResizeThickness * Vec2f::j);
    }

    void WindowEx::UpdateDraggableSize()
    {
        GetDraggable().SetSize(Vec2f(GetSize().x, GetBorderThickness().top - DefaultResizeThickness));
    }
}