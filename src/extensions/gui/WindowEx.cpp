#include <doge/extensions/gui/WindowEx.hpp>

#include <doge/extensions/gui/GUI.hpp>
#include <doge/extensions/gui/Text.hpp>
#include <doge/extensions/gui/Draggable.hpp>
#include <doge/extensions/gui/NSImage.hpp>
#include <doge/extensions/gui/Resizable.hpp>
#include <doge/extensions/gui/Button.hpp>
#include <doge/extensions/gui/Image.hpp>
#include <doge/extensions/gui/Clickable.hpp>

namespace doge::gui
{
    const Rectf WindowEx::DefaultResizeThickness = Rectf(5, 5, 5, 5);
    const Rectf WindowEx::DefaultBorderThickness = Rectf(16, 32, 16, 16);

    WindowEx::~WindowEx()
    {
    }

    void WindowEx::Initialize()
    {
        Window::Initialize();

        GetImage().SetTextureID("doge_gui_windowex");
        GetImage().SetBorderThickness(Rectf());
        SetBorderThickness(DefaultBorderThickness);
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
            title_bar.SetCursorDetectable(false);
            title_bar.SetAlign(Align::Center | Align::Left);
            title_bar.SetFontSize(14);
            title_bar.SetString(U"WindowEx Title");
            title_bar.SetAppearance(doge::Text::Appearance{ .fill_color = Color::Black });

            UpdateTitleBarOrigin();
            UpdateTitleBarLayer();
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
            draggable.SetCursorDetectable(true);
            draggable.SetAlign(Align::Top | Align::Center);

            draggable.on_drag_began += [&](const Vec2f& pos){ drag_start_pos = GetPosition(); };
            draggable.on_dragged_diff += [&](const Vec2f& diff){ SetPosition(drag_start_pos + diff); };

            UpdateDraggableOrigin();
            UpdateDraggableSize();
            UpdateDraggableLayer();
        }
    }

    bool WindowEx::IsDraggable() const
    {
        return trait_enabled.test(Trait::Draggable);        
    }

    void WindowEx::SetResizable(bool enabled)
    {
        auto prev_enabled = trait_enabled.test(Trait::Resizable);
        trait_enabled.set(Trait::Resizable, enabled);

        if (prev_enabled && !enabled)
        {
            GetGUI().RemoveElement(GetResizableElementID());
        }
        else if (!prev_enabled && enabled)
        {
            auto& resizable = GetGUI().AddElement<gui::Resizable>(GetResizableElementID(), GetCameraID());
            resizable.GetEntity().SetParent(GetEntity());
            resizable.SetCursorDetectable(true);
            resizable.SetThickness(DefaultResizeThickness);
            resizable.SetMinSize(Vec2f(100, 100));

            resizable.on_resize_began += [&](){ resize_start_pos = GetPosition(); };
            resizable.on_resized += 
            [&](const Vec2f& d_pos)
            {
                resize_guard = true;
                SetPosition(resize_start_pos + d_pos);
                SetSize(resizable.GetSize());
                resize_guard = false;
            };

            UpdateResizableOrigin();
            UpdateResizableLayer();
        }

        OnSizeUpdated();
        OnOriginUpdated();
    }

    bool WindowEx::IsResizable() const
    {
        return trait_enabled.test(Trait::Resizable);
    }

    void WindowEx::SetCloseButton(bool enabled)
    {
        auto prev_enabled = trait_enabled.test(Trait::CloseButton);
        trait_enabled.set(Trait::CloseButton, enabled);

        if (prev_enabled && !enabled)
        {
            GetGUI().RemoveElement(GetCloseButtonElementID());
        }
        else if (!prev_enabled && enabled)
        {
            auto& button = GetGUI().AddElement<Button>(GetCloseButtonElementID(), GetCameraID());
            button.GetEntity().SetParent(GetEntity());
            button.SetCursorDetectable(true);
            button.Set9Slice(false);
            button.GetImage().SetTextureID("doge_gui_windowex_close_button");
            button.SetAlign(Align::Center | Align::Right);
            button.SetSize(Vec2f(24, 24));

            button.GetClickable().on_clicked +=
            [&](io::Mouse::Button button)
            {
                if (button == io::Mouse::Button::Left)
                    GetGUI().RemoveElement(GetID());
            };
            
            button.on_state_transition = 
            [](Button& button)
            {
                if (button.GetClickable().IsDown(io::Mouse::Button::Left))
                {
                    button.SetColor(0xF6F6F6FF);
                    button.GetImage().SetTextureAtlasRectangle("pressed");
                }
                else if (button.GetClickable().IsMouseOver())
                {
                    button.SetColor(0xFFFFFFFF);
                    button.GetImage().SetTextureAtlasRectangle("pressed");
                }
                else
                {
                    button.SetColor(0xF3F3F3FF);
                    button.GetImage().SetTextureAtlasRectangle("default");
                }
            };

            button.on_state_transition(button);

            UpdateCloseButtonLayer();
            UpdateCloseButtonOrigin();
        }
    }

    bool WindowEx::HasCloseButton() const
    {
        return trait_enabled.test(Trait::CloseButton);
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

    std::string WindowEx::GetResizableElementID() const
    {
        return "window_gui_windowex_" + GetID() + "_resizable";
    }

    gui::Resizable& WindowEx::GetResizable() const
    {
        if (!IsResizable())
            throw std::invalid_argument("Resizable is not enabled");

        return static_cast<gui::Resizable&>(GetGUI().GetElement(GetResizableElementID()));
    }

    std::string WindowEx::GetCloseButtonElementID() const
    {
        return "window_gui_windowex_" + GetID() + "_close_button";
    }

    Button& WindowEx::GetCloseButton() const
    {
        if (!HasCloseButton())
            throw std::invalid_argument("CloseButton is not enabled");

        return static_cast<Button&>(GetGUI().GetElement(GetCloseButtonElementID()));
    }

    void WindowEx::SetBorderThickness(const Rectf& border_thickness)
    {
        Window::SetBorderThickness(border_thickness);

        if (IsDraggable())
        {
            UpdateDraggableSize();
        }

        if (HasCloseButton())
        {
            UpdateCloseButtonOrigin();
        }
    }

    void WindowEx::OnLayerUpdated()
    {
        Window::OnLayerUpdated();

        if (HasTitleBar())
        {
            UpdateTitleBarLayer();
        }

        if (IsDraggable())
        {
            UpdateDraggableLayer();
        }

        if (IsResizable())
        {
            UpdateResizableLayer();
        }

        if (HasCloseButton())
        {
            UpdateCloseButtonOrigin();
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
        
        if (IsResizable() && !resize_guard)
        {
            UpdateResizableSize();
        }

        if (HasCloseButton())
        {
            UpdateCloseButtonOrigin();
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

        if (IsResizable() && !resize_guard)
        {
            UpdateResizableOrigin();
        }

        if (HasCloseButton())
        {
            UpdateCloseButtonOrigin();
        }
    }

    void WindowEx::UpdateTitleBarLayer()
    {
        GetTitleBar().SetLayer(GetLayer() + 1);
    }
    
    void WindowEx::UpdateTitleBarOrigin()
    {
        GetTitleBar().SetOrigin(GetActualOrigin() - Vec2f(GetBorderThickness().left, GetBorderThickness().top / 2.f));
    }

    void WindowEx::UpdateDraggableLayer()
    {
        GetDraggable().SetLayer(GetLayer() + 1);
    }

    void WindowEx::UpdateDraggableOrigin()
    {
        GetDraggable().SetOrigin(GetActualOrigin() - Vec2f(GetSize().x / 2.f, IsResizable() ? GetResizable().GetThickness().top : 0.f));
    }

    void WindowEx::UpdateDraggableSize()
    {
        GetDraggable().SetSize(Vec2f(GetSize().x, GetBorderThickness().top - (IsResizable() ? GetResizable().GetThickness().top : 0.f)));
    }

    void WindowEx::UpdateResizableLayer()
    {
        GetResizable().SetLayer(GetLayer() + 1);
    }

    void WindowEx::UpdateResizableOrigin()
    {
        GetResizable().SetOrigin(GetOrigin());
        GetResizable().SetAlign(GetAlign());
    }

    void WindowEx::UpdateResizableSize()
    {
        GetResizable().SetSize(GetSize());
    }

    void WindowEx::UpdateCloseButtonLayer()
    {
        GetCloseButton().SetLayer(GetLayer() + 1);
    }

    void WindowEx::UpdateCloseButtonOrigin()
    {
        GetCloseButton().SetOrigin(GetActualOrigin() - Vec2f(GetSize().x - 4, GetBorderThickness().top / 2.f));
    }
}