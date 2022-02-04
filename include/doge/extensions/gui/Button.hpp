#pragma once

#include <doge/utils/aliases.hpp>
#include <doge/utils/Color.hpp>
#include <doge/core/Entity.hpp>
#include <doge/extensions/gui/Element.hpp>
#include <doge/extensions/gui/Text.hpp>
#include <functional>
#include <variant>

namespace doge::gui
{
    struct Image;
    struct NSImage;

    struct Button : Element
    {
        Event<> on_pressed;
        Event<> on_released;
        Event<> on_mouse_entered;
        Event<> on_mouse_left;
        Event<> on_clicked;
        std::function<void(Button&)> on_state_transition = Button::DefaultOnStateTransition;

        virtual ~Button();

        virtual void Initialize() override;

        void Set9Slice(bool is_9_slice);
        bool Is9Slice() const;

        std::string GetImageElementID() const;
        Element& GetImageElement() const;
        Image& GetImage() const;
        NSImage& GetNSImage() const;

        std::string GetTextElementID() const;
        Text& GetText() const;

        bool IsDown() const;
        bool IsMouseOver() const;

        static const Vec2f DefaultSize;
        static void DefaultOnStateTransition(Button& button);

    protected:

        virtual void OnSizeUpdated() override;
        virtual void OnPositionUpdated() override;
        virtual void OnOriginUpdated() override;
        virtual void OnColorUpdated() override;

    private:

        enum State
        {
            Down = 0,
            MouseOver = 1,
            Count
        };

        std::bitset<State::Count> states;
        bool is_9_slice = true;
    };
}