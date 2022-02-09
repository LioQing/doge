#pragma once

#include <doge/utils/aliases.hpp>
#include <doge/utils/Color.hpp>
#include <doge/core/Entity.hpp>
#include <doge/extensions/gui/Element.hpp>
#include <functional>
#include <variant>

namespace doge::gui
{
    struct Image;
    struct NSImage;
    struct Clickable;
    struct Text;

    struct Button : Element
    {
        std::function<void(Button&)> on_state_transition = Button::DefaultOnStateTransition;

        virtual ~Button();

        virtual void Initialize() override;

        void SetHasText(bool has_text);
        bool HasText() const;

        void Set9Slice(bool is_9_slice);
        bool Is9Slice() const;

        std::string GetImageElementID() const;
        Element& GetImageElement() const;
        Image& GetImage() const;
        NSImage& GetNSImage() const;

        std::string GetTextElementID() const;
        Text& GetText() const;

        std::string GetClickableElementID() const;
        Clickable& GetClickable() const;

        static const Vec2f DefaultSize;
        static void DefaultOnStateTransition(Button& button);

    protected:

        virtual void OnLayerUpdated() override;
        virtual void OnSizeUpdated() override;
        virtual void OnPositionUpdated() override;
        virtual void OnOriginUpdated() override;
        virtual void OnColorUpdated() override;

    private:

        bool is_9_slice = true;
        bool has_text = false;
    };
}