#pragma once

#include <doge/utils/aliases.hpp>
#include <doge/utils/Color.hpp>
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

        std::function<void(Button&)> on_state_transition = Button::DefaultOnStateTransition;

        void Initialize(Engine& engine) override;

        void SetTextureID(const std::string& texture_id);
        const std::string& GetTextureID() const;

        void SetIs9Slice(bool is_9_slice);
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

        bool IsDown() const;
        bool IsMouseOver() const;

        static void DefaultOnStateTransition(Button& button);

    protected:

        void OnSizeUpdated() override;
        void OnPositionUpdated() override;
        void OnOriginUpdated() override;

    private:

        enum State
        {
            Down = 0,
            MouseOver = 1,
            Count
        };

        std::bitset<State::Count> states;
        std::string texture_id = "doge_gui_button";
        bool is_9_slice = true;
        std::string atlas_rectangle_id = "";
        Recti texture_rectangle = Recti();
        Vec2i center_texture_size = Vec2i::Zero();
        Rectf border_thickness = Rectf();
        Color color = Color::White();
    };
}