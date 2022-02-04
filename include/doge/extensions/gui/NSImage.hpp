#pragma once

#include <doge/extensions/gui/Element.hpp>
#include <doge/utils/Rect.hpp>
#include <doge/utils/Color.hpp>

namespace doge::gui
{
    struct NSImage : Element
    {
        virtual ~NSImage();

        virtual void Initialize() override;

        void SetTextureID(const std::string& id);
        std::string GetTextureID() const;

        void SetCenterTextureSize(const Vec2i& center_texture_size);
        Vec2i GetCenterTextureSize() const;

        void SetBorderThickness(const Rectf& border_thickness);
        Rectf GetBorderThickness() const;

    protected:

        virtual void OnSizeUpdated() override;
        virtual void OnOriginUpdated() override;
        virtual void OnColorUpdated() override;
    };
}