#include <doge/extensions/gui/Button.hpp>

#include <doge/extensions/gui/GUI.hpp>
#include <doge/extensions/gui/NSImage.hpp>
#include <doge/extensions/gui/Image.hpp>
#include <doge/extensions/gui/Text.hpp>
#include <doge/extensions/gui/Clickable.hpp>
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
        auto& image = GetGUI().AddElement<gui::NSImage>(GetImageElementID(), GetCameraID());
        image.GetEntity().SetParent(GetEntity());
        image.SetAlign(Align::Top | Align::Left);
        image.SetTextureID("doge_gui_button");
        
        auto& clickable = GetGUI().AddElement<Clickable>(GetClickableElementID(), GetCameraID());
        clickable.GetEntity().SetParent(GetEntity());
        clickable.SetAlign(Align::Top | Align::Left);
        clickable.on_pressed        += [&](const Vec2f&, io::Mouse::Button){ on_state_transition(*this); };
        clickable.on_released       += [&](const Vec2f&, io::Mouse::Button){ on_state_transition(*this); };
        clickable.on_cursor_entered += [&](const Vec2f&){ on_state_transition(*this); };
        clickable.on_cursor_left    += [&](const Vec2f&){ on_state_transition(*this); };

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
        image_element.GetEntity().SetParent(GetEntity());
        image_element.SetSize(GetSize());
        image_element.SetOrigin(GetActualOrigin());
        image_element.SetColor(GetColor());
        image_element.SetLayer(GetLayer());
        image_element.SetAlign(Align::Top | Align::Left);

        on_state_transition(*this);
    }

    bool Button::Is9Slice() const
    {
        return is_9_slice;
    }

    void Button::SetHasText(bool has_text)
    {
        bool had_text = this->has_text;

        this->has_text = has_text;

        if (had_text && !has_text)
        {
            GetGUI().RemoveElement(GetTextElementID());
        }
        else if (!had_text && has_text)
        {
            auto& text = GetGUI().AddElement<gui::Text>(GetTextElementID(), GetCameraID());
            text.GetEntity().SetParent(GetEntity());
            text.SetString(U"Button");
            text.SetTextAlign(doge::Text::Center);
            text.SetAppearance(doge::Text::Appearance{ .fill_color = Color::Black });
        }

        OnLayerUpdated();
        OnOriginUpdated();
    }

    bool Button::HasText() const
    {
        return has_text;
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
        if (!HasText())
            throw std::invalid_argument("Button does not have gui::Text");

        return static_cast<Text&>(GetGUI().GetElement(GetTextElementID()));
    }

    std::string Button::GetClickableElementID() const
    {
        return "doge_gui_button_" + GetID() + "_clickable";
    }

    Clickable& Button::GetClickable() const
    {
        return static_cast<Clickable&>(GetGUI().GetElement(GetClickableElementID()));
    }

    void Button::DefaultOnStateTransition(Button& button)
    {
        if (button.GetClickable().IsDown(io::Mouse::Button::Left))
        {
            button.SetColor(0xF0F0F0FF);
        }
        else if (button.GetClickable().IsMouseOver())
        {
            button.SetColor(0xF6F6F6FF);
        }
        else
        {
            button.SetColor(0xF3F3F3FF);
        }
    }

    void Button::OnLayerUpdated()
    {
        GetImageElement().SetLayer(GetLayer());
        GetClickable().SetLayer(GetLayer() + 1);
        if (HasText()) GetText().SetLayer(GetLayer() + 1);
    }

    void Button::OnSizeUpdated()
    {
        GetImageElement().SetSize(GetSize());
        GetClickable().SetSize(GetSize());
    }

    void Button::OnOriginUpdated()
    {
        GetImageElement().SetOrigin(GetActualOrigin());
        GetClickable().SetOrigin(GetActualOrigin());
        if (HasText()) GetText().SetOrigin(GetActualOrigin() - GetSize() / 2.f);
    }

    void Button::OnColorUpdated()
    {
        GetImageElement().SetColor(GetColor());
    }
}