#include <doge/extensions/gui/Button.hpp>

#include <doge/extensions/gui/GUI.hpp>
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

        GetEntity().AddComponent<doge::Position>(0, 0);
        
        UpdateSprite();

        text_id = "doge_gui_button_" + GetID() + "_text";
        
        auto& text = GetGUI().AddElement<gui::Text>(text_id, GetCameraID());
        text.GetEntity().SetParent(GetEntity());
        text.SetString(U"Button");
        text.SetAlign(doge::Text::Align::Center);
        text.SetVerticalAlign(Text::VerticalAlign::Center);
        text.SetAppearance(doge::Text::Appearance{ .fill_color = Color::Black });

        on_state_transition(*this);

        SetSize(DefaultSize);
    }

    void Button::SetTextureID(const std::string& texture_id)
    {
        this->texture_id = texture_id;

        if (Is9Slice())
            GetGUI().GetNineSlice().SetSpriteTextureID(GetEntity().GetComponent<CompoundSprite>(), GetTextureID());
        else
            GetEntity().GetComponent<Sprite>().texture_id = GetTextureID();
    }

    const std::string& Button::GetTextureID() const
    {
        return texture_id;
    }

    void Button::SetIs9Slice(bool is_9_slice)
    {
        this->is_9_slice = is_9_slice;

        UpdateSprite();
    }

    bool Button::Is9Slice() const
    {
        return is_9_slice;
    }

    void Button::SetAtlasRectangleID(const std::string& id)
    {
        this->atlas_rectangle_id = id;

        if (!Is9Slice())
            GetEntity().GetComponent<Sprite>().atlas_rectangle_id = GetAtlasRectangleID();
    }

    const std::string& Button::GetAtlasRectangleID() const
    {
        return atlas_rectangle_id;
    }

    void Button::SetTextureRectangle(const Recti& texture_rectangle)
    {
        this->texture_rectangle = texture_rectangle;

        if (!Is9Slice())
            GetEntity().GetComponent<Sprite>().texture_rectangle = GetTextureRectangle();
    }

    const Recti& Button::GetTextureRectangle() const
    {
        return texture_rectangle;
    }

    void Button::SetCenterTextureSize(const Vec2i& center_texture_size)
    {
        this->center_texture_size = center_texture_size;

        if (Is9Slice())
            GetGUI().GetNineSlice().SetSpriteCenterTextureSize(GetEntity().GetComponent<CompoundSprite>(), GetCenterTextureSize());
    }

    const Vec2i& Button::GetCenterTextureSize() const
    {
        return center_texture_size;
    }

    void Button::SetBorderThickness(const Rectf& border_thickness)
    {
        this->border_thickness = border_thickness;

        if (Is9Slice())
            GetGUI().GetNineSlice().SetSpriteSizeAndBorder(GetEntity().GetComponent<CompoundSprite>(), GetSize(), GetBorderThickness());
    }

    const Rectf& Button::GetBorderThickness() const
    {
        return border_thickness;
    }

    void Button::SetColor(const Color& color)
    {
        this->color = color;

        if (Is9Slice())
            GetGUI().GetNineSlice().SetSpriteColor(GetEntity().GetComponent<CompoundSprite>(), GetColor());
        else
            GetEntity().GetComponent<Sprite>().color = GetColor();
    }

    const Color& Button::GetColor() const
    {
        return color;
    }

    const std::string& Button::GetTextElementID() const
    {
        return text_id;
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
        if (Is9Slice())
            GetGUI().GetNineSlice().SetSpriteSizeAndBorder(GetEntity().GetComponent<CompoundSprite>(), GetSize(), GetBorderThickness());
        else
            GetEntity().GetComponent<Sprite>().size = GetSize();
        
        SetOrigin(GetOrigin());
    }

    void Button::OnPositionUpdated()
    {
        GetEntity().GetComponent<doge::Position>().position = GetPosition();
    }

    void Button::OnOriginUpdated()
    {
        if (Is9Slice())
            GetGUI().GetNineSlice().SetSpriteOrigin(GetEntity().GetComponent<CompoundSprite>(), GetOrigin() + GetSize() / 2.f);
        else
            GetEntity().GetComponent<Sprite>().origin = GetOrigin() + GetSize() / 2.f;
    }

    void Button::UpdateSprite()
    {
        if (Is9Slice())
        {
            GetGUI().GetNineSlice().Add9SliceSpriteBySize(
                GetEntity(),
                GetTextureID(),
                GetSize(),
                GetCenterTextureSize(),
                GetBorderThickness(),
                GetOrigin() + GetSize() / 2.f,
                GetColor()
            );
        }
        else
        {
            GetEntity().AddComponent(Sprite
            {
                .texture_id = GetTextureID(),
                .atlas_rectangle_id = GetAtlasRectangleID(),
                .texture_rectangle = GetTextureRectangle(),
                .size = GetSize(),
                .origin = GetOrigin() + GetSize() / 2.f,
                .color = GetColor()
            });
        }
    }
}