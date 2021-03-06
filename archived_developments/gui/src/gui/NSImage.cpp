#include <doge/extensions/gui/NSImage.hpp>

#include <doge/extensions/nine_slice.hpp>
#include <doge/extensions/gui/GUI.hpp>
#include <doge/utils/math.hpp>

namespace doge::gui
{
    NSImage::~NSImage()
    {
    }

    void NSImage::Initialize()
    {
        GetGUI().GetNineSlice().Add9SliceSpriteBySize(GetEntity());

        OnOriginUpdated();
    }

    void NSImage::SetTextureID(const std::string& id)
    {
        GetGUI().GetNineSlice().SetSpriteTextureID(GetEntity().GetComponent<CompoundSprite>(), id);
        OnOriginUpdated();
    }

    std::string NSImage::GetTextureID() const
    {
        return GetGUI().GetNineSlice().GetSpriteTextureID(GetEntity().GetComponent<CompoundSprite>());
    }

    void NSImage::SetCenterTextureSize(const Vec2i& center_texture_size)
    {
        GetGUI().GetNineSlice().SetSpriteCenterTextureSize(GetEntity().GetComponent<CompoundSprite>(), center_texture_size);
    }

    Vec2i NSImage::GetCenterTextureSize() const
    {
        return GetGUI().GetNineSlice().GetSpriteCenterTextureSize(GetEntity().GetComponent<CompoundSprite>());
    }

    void NSImage::SetBorderThickness(const Rectf& border_thickness)
    {
        GetGUI().GetNineSlice().SetSpriteSizeAndBorder(GetEntity().GetComponent<CompoundSprite>(), GetSize(), border_thickness);
    }

    Rectf NSImage::GetBorderThickness() const
    {
        return GetGUI().GetNineSlice().GetSpriteBorderThickness(GetEntity().GetComponent<CompoundSprite>());
    }

    void NSImage::OnSizeUpdated()
    {
        GetGUI().GetNineSlice().SetSpriteSizeAndBorder(GetEntity().GetComponent<CompoundSprite>(), GetSize(), GetBorderThickness());
    }

    void NSImage::OnOriginUpdated()
    {
        GetGUI().GetNineSlice().SetSpriteOrigin(GetEntity().GetComponent<CompoundSprite>(), GetActualOrigin());
    }

    void NSImage::OnColorUpdated()
    {
        GetGUI().GetNineSlice().SetSpriteColor(GetEntity().GetComponent<CompoundSprite>(), GetColor());
    }
}