#pragma once

#include <doge/extensions/gui/Element.hpp>
#include <doge/components/Text.hpp>

namespace doge::gui
{
    struct Text : Element
    {
        virtual ~Text();

        virtual void Initialize() override;

        void SetString(const std::u32string& str);
        const std::u32string& GetString() const;

        void SetFont(const std::string& font_id);
        const std::string& GetTextFontID() const;

        void SetFontSize(std::uint32_t font_size);
        std::uint32_t GetFontSize() const;

        void SetLineSpacingFactor(float factor);
        float GetLineSpacingFactor() const;

        void SetTextAlign(doge::Text::Align align);
        doge::Text::Align GetTextAlign() const;

        void SetAppearance(const doge::Text::Appearance& appear, std::size_t pos = 0);
        const doge::Text::Appearance& GetAppearance(std::size_t pos = 0) const;

    protected:

        virtual void OnOriginUpdated() override;
        virtual void OnColorUpdated() override;
    };
}