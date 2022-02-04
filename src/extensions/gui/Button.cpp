#include <doge/extensions/gui/Button.hpp>

#include <doge/extensions/gui/GUI.hpp>
#include <doge/extensions/gui/NSImage.hpp>
#include <doge/extensions/gui/Image.hpp>
#include <doge/extensions/nine_slice.hpp>
#include <doge/core/Engine.hpp>
#include <doge/utils/math.hpp>
#include <algorithm>

namespace doge::gui
{
    const Vec2f Button::DefaultSize = Vec2f(100, 36);

    Button::~Button()
    {
    }

    void Button::Initialize()
    {
        GetGUI().GetEngine().events.on_mouse_button_pressed.AddListener(std::string("doge_gui_button_" + GetID()),
        [&](const event::MouseButton& event)
        {
            if (
                event.button == event::MouseButton::Button::Left && 
                GetGUI().GetElementBelowCursor().get() == static_cast<Element*>(this)
            )
            {
                on_pressed();
                states.set(State::Down, true);
                on_state_transition(*this);
            }
        });

        GetGUI().GetEngine().events.on_mouse_button_released.AddListener(std::string("doge_gui_button_" + GetID()),
        [&](const event::MouseButton& event)
        {
            if (event.button == event::MouseButton::Button::Left)
            {
                if (states.test(State::Down))
                {
                    on_released();
                    if (GetGUI().GetElementBelowCursor().get() == static_cast<Element*>(this))
                        on_clicked();
                }

                states.set(State::Down, false);
                on_state_transition(*this);
            }
        });

        GetGUI().GetEngine().events.on_mouse_moved.AddListener(std::string("doge_gui_button_" + GetID()),
        [&](const event::MouseMove& event)
        {
            if (
                !states.test(State::MouseOver) &&
                GetGUI().GetElementBelowCursor().get() == static_cast<Element*>(this)
            )
            {
                on_mouse_entered();
                states.set(State::MouseOver, true);
                on_state_transition(*this);
            }
            else if (
                states.test(State::MouseOver) &&
                GetGUI().GetElementBelowCursor().get() != static_cast<Element*>(this)
            )
            {
                on_mouse_left();
                states.set(State::MouseOver, false);
                on_state_transition(*this);
            }
        });

        GetEntity().GetComponent<doge::gui::Component>().OnRemoval([&]
        {
            GetGUI().GetEngine().events.on_mouse_button_pressed.RemoveListener(std::string("doge_gui_button_" + GetID()));
            GetGUI().GetEngine().events.on_mouse_button_released.RemoveListener(std::string("doge_gui_button_" + GetID()));
            GetGUI().GetEngine().events.on_mouse_moved.RemoveListener(std::string("doge_gui_button_" + GetID()));
        });

        auto& image = GetGUI().AddElement<gui::NSImage>(GetImageElementID(), GetCameraID());
        image.GetEntity().SetParent(GetEntity());
        image.SetLocalLayer(0);
        image.SetCursorDetectable(false);
        image.SetTextureID("doge_gui_button");
        
        auto& text = GetGUI().AddElement<gui::Text>(GetTextElementID(), GetCameraID());
        text.GetEntity().SetParent(GetEntity());
        text.SetLocalLayer(1);
        text.SetCursorDetectable(false);
        text.SetString(U"Button");
        text.SetAlign(doge::Text::Align::Center);
        text.SetVerticalAlign(Text::VerticalAlign::Center);
        text.SetAppearance(doge::Text::Appearance{ .fill_color = Color::Black });

        on_state_transition(*this);

        SetSize(DefaultSize);
    }

    void Button::Set9Slice(bool is_9_slice)
    {
        bool changed = this->is_9_slice != is_9_slice;

        this->is_9_slice = is_9_slice;

        if (!changed)
            return;

        GetGUI().RemoveElement(GetImageElementID());
        if (this->is_9_slice)
            GetGUI().AddElement<NSImage>(GetImageElementID(), GetCameraID());
        else
            GetGUI().AddElement<Image>(GetImageElementID(), GetCameraID());
        
        auto& image_element = GetImageElement();
        image_element.SetSize(GetSize());
        image_element.SetOrigin(GetOrigin());
        image_element.SetColor(GetColor());
    }

    bool Button::Is9Slice() const
    {
        return is_9_slice;
    }
    
    std::string Button::GetImageElementID() const
    {
        return "doge_gui_button_" + GetID() + "_image";
    }
    
    Element& Button::GetImageElement() const
    {
        return GetGUI().GetElement(GetImageElementID());
    }

    Image& Button::GetImage() const
    {
        if (Is9Slice())
            throw std::invalid_argument("Button is 9 slice, call GetNSImage() instead");

        return static_cast<Image&>(GetImageElement());
    }

    NSImage& Button::GetNSImage() const
    {
        if (!Is9Slice())
            throw std::invalid_argument("Button is not 9 slice, call GetImage() instead");

        return static_cast<NSImage&>(GetImageElement());
    }

    std::string Button::GetTextElementID() const
    {
        return "doge_gui_button_" + GetID() + "_text";
    }

    Text& Button::GetText() const
    {
        return static_cast<Text&>(GetGUI().GetElement(GetTextElementID()));
    }

    bool Button::IsDown() const
    {
        return states.test(State::Down);
    }

    bool Button::IsMouseOver() const
    {
        return states.test(State::MouseOver);
    }

    void Button::DefaultOnStateTransition(Button& button)
    {
        if (button.IsDown())
        {
            button.SetColor(0xF0F0F0FF);
        }
        else if (button.IsMouseOver())
        {
            button.SetColor(0xF6F6F6FF);
        }
        else
        {
            button.SetColor(0xF3F3F3FF);
        }
    }

    void Button::OnSizeUpdated()
    {
        GetImageElement().SetSize(GetSize());
    }

    void Button::OnPositionUpdated()
    {
        GetEntity().GetComponent<doge::Position>().position = GetPosition();
    }

    void Button::OnOriginUpdated()
    {
        GetImageElement().SetOrigin(GetOrigin());
        GetText().SetOrigin(GetOrigin());
    }

    void Button::OnColorUpdated()
    {
        GetImageElement().SetColor(GetColor());
    }
}