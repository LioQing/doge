#include <doge/extensions/gui/Window.hpp>

#include <doge/extensions/gui/GUI.hpp>
#include <algorithm>

namespace doge::gui
{
    const Vec2f Window::DefaultSize = Vec2f(600, 480);

    void Window::Initialize()
    {
        // camera
        camera_entity = GetGUI().AddCamera(GetWindowCameraID());
        camera_entity.SetParent(GetEntity());

        // camera
        auto& cam_comp = camera_entity.GetComponent<Camera>();
        cam_comp.render_order = GetGUI().GetCameraRenderOrder(GetCameraID()) + 1;

        cam_comp.OnRemoval([&]()
        {
            GetGUI().RemoveElements(GetWindowCameraID());
        });

        // cam layer
        auto& parent_cam_layers = GetGUI().GetCameraLayers(GetCameraID());
        auto [min_itr, max_itr] = std::minmax_element(parent_cam_layers.begin(), parent_cam_layers.end());
        std::set<std::int32_t> layers;
        for (auto& layer : parent_cam_layers)
            layers.emplace(layer + *max_itr - *min_itr + 1);
        
        camera_entity.AddComponent<Layer>(layers);

        // sprite
        GetGUI().GetNineSlice().Add9SliceSpriteBySize(
            GetEntity(),
            "doge_gui_window",
            GetSize(),
            Vec2i::Zero,
            Rectf(),
            GetSize() / 2.f,
            0xECECECFF
        );

        SetSize(DefaultSize);
    }

    std::int32_t Window::GetLayer() const
    {
        return GetGUI().GetCameraLayer(GetCameraID()) + 2;
    }

    std::string Window::GetWindowCameraID() const
    {
        return std::string("doge_gui_window_") + GetID();
    }

    void Window::SetTextureID(const std::string& texture_id)
    {
        GetGUI().GetNineSlice().SetSpriteTextureID(GetEntity().GetComponent<CompoundSprite>(), texture_id);
    }

    std::string Window::GetTextureID() const
    {
        return GetGUI().GetNineSlice().GetSpriteTextureID(GetEntity().GetComponent<CompoundSprite>());
    }

    void Window::SetContainerBorderThickness(const Rectf& border_thickness)
    {
        this->border_thickness = border_thickness;

        GetGUI().GetNineSlice().SetSpriteSizeAndBorder(GetEntity().GetComponent<CompoundSprite>(), GetSize(), GetBorderThickness());
        UpdateContainerArea();
    }

    const Rectf& Window::GetContainerBorderThickness() const
    {
        return border_thickness;
    }

    void Window::SetCenterTextureSize(const Vec2i& center_texture_size)
    {
        GetGUI().GetNineSlice().SetSpriteCenterTextureSize(GetEntity().GetComponent<CompoundSprite>(), center_texture_size);
    }

    Vec2i Window::GetCenterTextureSize() const
    {
        return GetGUI().GetNineSlice().GetSpriteCenterTextureSize(GetEntity().GetComponent<CompoundSprite>());
    }

    void Window::SetBorderThickness(const Rectf& border_thickness)
    {
        GetGUI().GetNineSlice().SetSpriteSizeAndBorder(GetEntity().GetComponent<CompoundSprite>(), GetSize(), border_thickness);
    }

    Rectf Window::GetBorderThickness() const
    {
        return GetGUI().GetNineSlice().GetSpriteBorderThickness(GetEntity().GetComponent<CompoundSprite>());
    }

    void Window::SetColor(const Color& color)
    {
        GetGUI().GetNineSlice().SetSpriteColor(GetEntity().GetComponent<CompoundSprite>(), color);
    }

    const Color& Window::GetColor() const
    {
        return GetGUI().GetNineSlice().GetSpriteColor(GetEntity().GetComponent<CompoundSprite>());
    }

    void Window::OnSizeUpdated()
    {
        GetGUI().GetNineSlice().SetSpriteSizeAndBorder(GetEntity().GetComponent<CompoundSprite>(), GetSize(), GetBorderThickness());
        GetGUI().GetNineSlice().SetSpriteOrigin(GetEntity().GetComponent<CompoundSprite>(), GetOrigin() + GetSize() / 2.f);
        UpdateContainerArea();
    }

    void Window::UpdateContainerArea()
    {
        auto& cam_comp = camera_entity.GetComponent<Camera>();
        cam_comp.size = GetSize() - GetBorderThickness().GetPosition() - GetBorderThickness().GetSize();
        cam_comp.port.SetPosition(Vec2f(0.5, 0.5) - (GetSize() / 2.f - GetBorderThickness().GetPosition()) / GetCameraComponent().size);
        cam_comp.port.SetSize(cam_comp.size / GetCameraComponent().size);
    }
}