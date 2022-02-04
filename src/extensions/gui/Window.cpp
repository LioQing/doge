#include <doge/extensions/gui/Window.hpp>

#include <doge/extensions/gui/GUI.hpp>
#include <doge/extensions/gui/NSImage.hpp>
#include <algorithm>

namespace doge::gui
{
    const Vec2f Window::DefaultSize = Vec2f(600, 480);

    Window::~Window()
    {
    }

    void Window::Initialize()
    {
        // camera
        auto camera_entity = GetGUI().AddCamera(
            GetWindowCameraID(),
            Rectf(0, 0, 1, 1),
            GetGUI().GetCameraRenderOrder(GetCameraID()) + 1,
            GetGUI().GetCameraLayer(GetCameraID()) + GetGUI().GetCameraLayerWidth(GetCameraID())
        );

        GetEntity().OnComponentRemoval<EntityInfo>([&, camera_entity]()
        {
            GetGUI().GetEngine().DestroyEntity(camera_entity);
        });

        GetGUI().GetEngine().events.on_window_resized.AddListener(
            std::string("doge_gui_window_") + GetID(),
            [&](const event::Size& event)
            {
                UpdateContainerArea();
            }
        );

        camera_entity.GetComponent<Camera>().OnRemoval([&]()
        {
            GetGUI().GetEngine().events.on_window_resized.RemoveListener(std::string("doge_gui_window_") + GetID());
            GetGUI().RemoveElements(GetWindowCameraID());
        });

        // image
        auto& image = GetGUI().AddElement<gui::NSImage>(GetImageElementID(), GetCameraID());
        image.GetEntity().SetParent(GetEntity());
        image.SetLocalLayer(2);
        image.SetCursorDetectable(false);
        image.SetTextureID("doge_gui_window");

        SetLocalLayer(2);
        SetSize(DefaultSize);
    }

    std::string Window::GetWindowCameraID() const
    {
        return "doge_gui_window_" + GetID();
    }

    void Window::SetWindowCameraRenderOrder(std::int32_t render_order)
    {
        GetGUI().GetCameraComponent(GetWindowCameraID()).render_order = GetGUI().GetCameraRenderOrder(GetCameraID()) + 1;
    }

    std::int32_t Window::GetWindowCameraRenderOrder() const
    {
        return GetGUI().GetCameraComponent(GetWindowCameraID()).render_order;
    }

    void Window::SetBorderThickness(const Rectf& border_thickness)
    {
        this->border_thickness = border_thickness;

        UpdateContainerArea();
    }

    const Rectf& Window::GetBorderThickness() const
    {
        return border_thickness;
    }

    std::string Window::GetImageElementID() const
    {
        return "doge_gui_window_" + GetID() + "_image";
    }

    NSImage& Window::GetImage() const
    {
        return static_cast<NSImage&>(GetGUI().GetElement(GetImageElementID()));
    }

    void Window::OnSizeUpdated()
    {
        GetImage().SetSize(GetSize());
        UpdateContainerArea();
    }

    void Window::OnPositionUpdated()
    {
        UpdateContainerArea();
    }

    void Window::OnOriginUpdated()
    {
        GetImage().SetOrigin(GetOrigin());
        UpdateContainerArea();
    }

    void Window::UpdateContainerArea()
    {
        auto& cam_comp = GetGUI().GetCameraComponent(GetWindowCameraID());
        cam_comp.size = GetSize() - GetBorderThickness().GetPosition() - GetBorderThickness().GetSize();
        cam_comp.port.SetPosition(Vec2f(0.5, 0.5) - (GetSize() / 2.f - GetBorderThickness().GetPosition() - GetPosition()) / GetCameraComponent().size);
        cam_comp.port.SetSize(cam_comp.size / GetCameraComponent().size);
    }
}