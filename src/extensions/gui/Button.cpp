#include <doge/extensions/gui/Button.hpp>

#include <doge/extensions/gui/gui.hpp>
#include <doge/extensions/nine_slice.hpp>
#include <doge/core/Engine.hpp>
#include <doge/utils/math.hpp>
#include <algorithm>

namespace doge
{
    void Button::Initialize()
    {
        gui::GetEngine().events.on_mouse_button_pressed +=
        [&](const event::MouseButton& event)
        {
            if (
                event.button == event::MouseButton::Button::Left && 
                math::TestPoint(gui::GetEngine().window.MapPixelToCoords(event.position, GetCameraComponent()), GetRectangle())
            )
            {
                on_pressed();
                states.set(State::Down, true);
                on_state_transition(*this);
            }
        };

        gui::GetEngine().events.on_mouse_button_released +=
        [&](const event::MouseButton& event)
        {
            if (
                event.button == event::MouseButton::Button::Left && 
                math::TestPoint(gui::GetEngine().window.MapPixelToCoords(event.position, GetCameraComponent()), GetRectangle())
            )
            {
                on_released();
                if (states.test(State::Down))
                    on_clicked();
            }

            states.set(State::Down, false);
            on_state_transition(*this);
        };

        gui::GetEngine().events.on_mouse_moved +=
        [&](const event::MouseMove& event)
        {
            if (
                !states.test(State::MouseOver) &&
                math::TestPoint(gui::GetEngine().window.MapPixelToCoords(event.position, GetCameraComponent()), GetRectangle())
            )
            {
                on_mouse_entered();
                states.set(State::MouseOver, true);
                on_state_transition(*this);
            }
            else if (
                states.test(State::MouseOver) &&
                !math::TestPoint(gui::GetEngine().window.MapPixelToCoords(event.position, GetCameraComponent()), GetRectangle())
            )
            {
                on_mouse_left();
                states.set(State::MouseOver, false);
                on_state_transition(*this);
            }
        };

        auto entity = gui::GetElementEntity(GetID());
        entity.AddComponent(Layer::Create(gui::GetCameraLayer(GetCameraID())));
        entity.AddComponent<doge::Position>(GetPosition());
        
        InitializeSpriteComponent(entity);

        text_entity = gui::GetEngine().AddEntity();
        text_entity.SetParent(entity);
        text_entity.AddComponent(Tag::Create(GetID() + "_text"));
        text_entity.AddComponent(Layer::Create(gui::GetCameraLayer(GetCameraID()) + 1));
        text_entity.AddComponent(Text
        {
            .font_id = "",
            .string = U"Button",
            .align = Text::Align::Center,
            .character_appearances = { std::pair<std::size_t, Text::Appearance>(0, Text::Appearance{ .fill_color = Color::Black }) },
        });

        on_state_transition(*this);

        SetSize(Vec2f(100, 36));
    }

    void Button::SetTextureID(const std::string& texture_id)
    {
        this->texture_id = texture_id;

        if (Is9Slice())
            nine_slice::SetSpriteTextureID(gui::GetElementEntity(GetID()).GetComponent<CompoundSprite>(), GetTextureID());
        else
            gui::GetElementEntity(GetID()).GetComponent<Sprite>().texture_id = GetTextureID();
    }

    const std::string& Button::GetTextureID() const
    {
        return texture_id;
    }

    void Button::SetIs9Slice(bool is_9_slice)
    {
        this->is_9_slice = is_9_slice;

        InitializeSpriteComponent(gui::GetElementEntity(GetID()));
    }

    bool Button::Is9Slice() const
    {
        return is_9_slice;
    }

    void Button::SetAtlasRectangleID(const std::string& id)
    {
        this->atlas_rectangle_id = id;

        if (gui::HasElement(GetID()) && !Is9Slice())
            gui::GetElementEntity(GetID()).GetComponent<Sprite>().atlas_rectangle_id = GetAtlasRectangleID();
    }

    const std::string& Button::GetAtlasRectangleID() const
    {
        return atlas_rectangle_id;
    }

    void Button::SetTextureRectangle(const Recti& texture_rectangle)
    {
        this->texture_rectangle = texture_rectangle;

        if (gui::HasElement(GetID()) && !Is9Slice())
            gui::GetElementEntity(GetID()).GetComponent<Sprite>().texture_rectangle = GetTextureRectangle();
    }

    const Recti& Button::GetTextureRectangle() const
    {
        return texture_rectangle;
    }

    void Button::SetCenterTextureSize(const Vec2i& center_texture_size)
    {
        this->center_texture_size = center_texture_size;

        if (gui::HasElement(GetID()) && Is9Slice())
            nine_slice::SetSpriteCenterTextureSize(gui::GetElementEntity(GetID()).GetComponent<CompoundSprite>(), GetCenterTextureSize());
    }

    const Vec2i& Button::GetCenterTextureSize() const
    {
        return center_texture_size;
    }

    void Button::SetBorderThickness(const Rectf& border_thickness)
    {
        this->border_thickness = border_thickness;

        if (gui::HasElement(GetID()) && Is9Slice())
            nine_slice::SetSpriteSizeAndBorder(gui::GetElementEntity(GetID()).GetComponent<CompoundSprite>(), GetSize(), GetBorderThickness());
    }

    const Rectf& Button::GetBorderThickness() const
    {
        return border_thickness;
    }

    void Button::SetColor(const Color& color)
    {
        this->color = color;

        if (Is9Slice())
            nine_slice::SetSpriteColor(gui::GetElementEntity(GetID()).GetComponent<CompoundSprite>(), GetColor());
        else
            gui::GetElementEntity(GetID()).GetComponent<Sprite>().color = GetColor();
    }

    const Color& Button::GetColor() const
    {
        return color;
    }

    void Button::SetText(const std::u32string& text)
    {
        text_entity.GetComponent<Text>().string = text;
        UpdateTextOrigin();
    }

    const std::u32string& Button::GetText() const
    {
        return text_entity.GetComponent<Text>().string;
    }

    void Button::SetTextFont(const std::string& font_id)
    {
        text_entity.GetComponent<Text>().font_id = font_id;
        UpdateTextOrigin();
    }

    const std::string& Button::GetTextFontID() const
    {
        return text_entity.GetComponent<Text>().font_id;
    }

    void Button::SetTextFontSize(std::uint32_t font_size)
    {
        text_entity.GetComponent<Text>().font_size = font_size;
        UpdateTextOrigin();
    }

    std::uint32_t Button::GetTextFontSize() const
    {
        return text_entity.GetComponent<Text>().font_size;
    }

    void Button::SetTextAppearance(const Text::Appearance& appear)
    {
        text_entity.GetComponent<Text>().character_appearances.at(0) = appear;
        UpdateTextOrigin();
    }

    const Text::Appearance& Button::GetTextAppearance() const
    {
        return text_entity.GetComponent<Text>().character_appearances.at(0);
    }

    bool Button::IsDown() const
    {
        return states.test(State::Down);
    }

    bool Button::IsMouseOver() const
    {
        return states.test(State::MouseOver);
    }

    void Button::UpdateTextOrigin()
    {
        auto& text = text_entity.GetComponent<Text>();
        if (text.font_id == "")
        {
            text.origin = Vec2f::Zero;
            return;
        }

        auto height = gui::GetEngine().assets.GetFont(text.font_id).GetLineSpacing(text.font_size) * text.line_spacing_factor;
        auto line = std::count(text.string.begin(), text.string.end(), U'\n') + 1;

        text.origin = Vec2f(0, line * height / 2.f);
    }

    void Button::DefaultOnStateTransition(Button& button)
    {
        if (button.IsDown())
        {
            button.SetColor(0xAAAAAAFF);
        }
        else if (button.IsMouseOver())
        {
            button.SetColor(0xEEEEEEFF);
        }
        else
        {
            button.SetColor(0xCFCFCFFF);
        }
    }

    void Button::OnSizeUpdated()
    {
        if (gui::HasElement(GetID()))
        {
            if (Is9Slice())
                nine_slice::SetSpriteSizeAndBorder(gui::GetElementEntity(GetID()).GetComponent<CompoundSprite>(), GetSize(), GetBorderThickness());
            else
                gui::GetElementEntity(GetID()).GetComponent<Sprite>().size = GetSize();
            
            SetOrigin(GetOrigin());
        }
    }

    void Button::OnPositionUpdated()
    {
        if (gui::HasElement(GetID()))
        {
            gui::GetElementEntity(GetID()).GetComponent<doge::Position>().position = GetPosition();
        }
    }

    void Button::OnOriginUpdated()
    {
        if (gui::HasElement(GetID()))
        {
            if (Is9Slice())
                nine_slice::SetSpriteOrigin(gui::GetElementEntity(GetID()).GetComponent<CompoundSprite>(), GetOrigin() + GetSize() / 2.f);
            else
                gui::GetElementEntity(GetID()).GetComponent<Sprite>().origin = GetOrigin() + GetSize() / 2.f;
        }
    }

    void Button::InitializeSpriteComponent(EntityID entity_id)
    {
        if (Is9Slice())
        {
            nine_slice::Add9SliceSpriteBySize(
                gui::GetEngine().assets,
                gui::GetEngine().GetEntity(entity_id),
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
            gui::GetEngine().GetEntity(entity_id).AddComponent(Sprite
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