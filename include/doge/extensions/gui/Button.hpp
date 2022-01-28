#pragma once

#include <doge/utils/aliases.hpp>
#include <doge/utils/Color.hpp>
#include <doge/core/Entity.hpp>
#include <doge/components/Text.hpp>
#include <doge/extensions/gui/GUIElement.hpp>
#include <functional>
#include <variant>

namespace doge
{
    struct Button : GUIElement
    {
        Event<> on_pressed;
        Event<> on_released;
        Event<> on_mouse_entered;
        Event<> on_mouse_left;
        Event<> on_clicked;

        void Initialize(Engine& engine) override;

        template <std::invocable<Button&> T>
        void SetOnStateTransition(T transition)
        {
            on_state_transition = transition;
            transition(*this);
        }

        void SetTextureID(const std::string& texture_id);
        const std::string& GetTextureID() const;

        void SetIs9Slice(Engine& engine, bool is_9_slice);
        bool Is9Slice() const;

        void SetAtlasRectangleID(const std::string& id);
        const std::string& GetAtlasRectangleID() const;

        void SetTextureRectangle(const Recti& texture_rectangle);
        const Recti& GetTextureRectangle() const;

        void SetCenterTextureSize(const Vec2i& center_texture_size);
        const Vec2i& GetCenterTextureSize() const;

        void SetBorderThickness(const Rectf& border_thickness);
        const Rectf& GetBorderThickness() const;

        void SetColor(const Color& color);
        const Color& GetColor() const;

        void SetText(const std::u32string& text);
        const std::u32string& GetText() const;

        void SetTextFont(const std::string& font_id);
        const std::string& GetTextFontID() const;

        void SetTextAppearance(const Text::Appearance& appear);
        const Text::Appearance& GetTextAppearance() const;

        bool IsDown() const;
        bool IsMouseOver() const;

        static void DefaultOnStateTransition(Button& button);

    protected:

        void OnSizeUpdated() override;
        void OnPositionUpdated() override;
        void OnOriginUpdated() override;

    private:

        void InitializeSpriteComponent(Engine& engine, EntityID entity_id);

        enum State
        {
            Down = 0,
            MouseOver = 1,
            Count
        };

        std::bitset<State::Count> states;
        std::function<void(Button&)> on_state_transition = Button::DefaultOnStateTransition;

        std::string texture_id = "doge_gui_button";
        bool is_9_slice = true;
        std::string atlas_rectangle_id = "";
        Recti texture_rectangle = Recti();
        Vec2i center_texture_size = Vec2i::Zero;
        Rectf border_thickness = Rectf();
        Color color = Color::White;

        Entity text_entity;
    };
}