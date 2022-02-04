#pragma once

#include <doge/extensions/gui/Element.hpp>
#include <doge/utils/Rect.hpp>
#include <doge/utils/Color.hpp>

namespace doge::gui
{
    struct Image : Element
    {
        virtual ~Image();

        virtual void Initialize() override;

        void SetTextureID(const std::string& id);
        const std::string& GetTextureID() const;

        void SetTextureAtlasRectangle(const std::string& id);
        const std::string& GetTextureAtlasRectangle() const;

        void SetTextureRectangle(const Recti& rectangle);
        const Recti& GetTextureRectangle() const;

    protected:

        virtual void OnSizeUpdated() override;
        virtual void OnOriginUpdated() override;
        virtual void OnColorUpdated() override;
    };
}