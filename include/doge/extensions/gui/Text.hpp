#pragma once

#include <doge/extensions/gui/Element.hpp>
#include <doge/components/Text.hpp>

namespace doge::gui
{
    struct Text : Element
    {
        enum VerticalAlign
        {
            Top, Center, Bottom,
        };

        virtual void Initialize() override;

        virtual std::int32_t GetLayer() const override;

        void SetString(const std::u32string& str);
        const std::u32string& GetString() const;

        void SetFont(const std::string& font_id);
        const std::string& GetTextFontID() const;

        void SetFontSize(std::uint32_t font_size);
        std::uint32_t GetFontSize() const;

        void SetLineSpacingFactor(float factor);
        float GetLineSpacingFactor() const;

        void SetAlign(doge::Text::Align align);
        doge::Text::Align GetAlign() const;

        void SetVerticalAlign(VerticalAlign vertical_align);
        VerticalAlign GetVerticalAlign() const;

        void SetAppearance(const doge::Text::Appearance& appear, std::size_t pos = 0);
        const doge::Text::Appearance& GetAppearance(std::size_t pos = 0) const;

    protected:

        virtual void OnPositionUpdated() override;
        virtual void OnOriginUpdated() override;

    private:

        VerticalAlign vertical_align = VerticalAlign::Top;
    };
}