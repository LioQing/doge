#include <doge/extensions/gui/Image.hpp>

#include <doge/extensions/gui/GUI.hpp>
#include <doge/utils/math.hpp>

namespace doge::gui
{
    Image::~Image()
    {
    }

    void Image::Initialize()
    {
        GetEntity().AddComponent<Sprite>();

        OnOriginUpdated();
    }

    void Image::SetTextureID(const std::string& id)
    {
        GetEntity().GetComponent<Sprite>().texture_id = id;
        OnOriginUpdated();
    }

    const std::string& Image::GetTextureID() const
    {
        return GetEntity().GetComponent<Sprite>().texture_id;
    }

    void Image::SetTextureAtlasRectangle(const std::string& id)
    {
        GetEntity().GetComponent<Sprite>().atlas_rectangle_id = id;
    }

    const std::string& Image::GetTextureAtlasRectangle() const
    {
        return GetEntity().GetComponent<Sprite>().atlas_rectangle_id;
    }

    void Image::SetTextureRectangle(const Recti& rectangle)
    {
        GetEntity().GetComponent<Sprite>().texture_rectangle = rectangle;
    }

    const Recti& Image::GetTextureRectangle() const
    {
        return GetEntity().GetComponent<Sprite>().texture_rectangle;
    }

    void Image::OnSizeUpdated()
    {
        GetEntity().GetComponent<Sprite>().size = GetSize();
        OnOriginUpdated();
    }

    void Image::OnOriginUpdated()
    {
        GetEntity().GetComponent<Sprite>().origin = GetOrigin() + GetEntity().GetComponent<Sprite>().GetActualSize(GetGUI().GetEngine().assets) / 2.f;
    }

    void Image::OnColorUpdated()
    {
        GetEntity().GetComponent<Sprite>().color = GetColor();
    }
}