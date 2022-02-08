#include <doge/extensions/gui/Window.hpp>

#include <doge/extensions/gui/GUI.hpp>
#include <doge/extensions/gui/Camera.hpp>
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
        auto& camera = GetGUI().AddCamera(GetWindowCameraID());

        GetEntity().OnComponentRemoval<EntityInfo>([&, camera_entity = camera.GetEntity()]()
        {
            GetGUI().GetEngine().DestroyEntity(camera_entity);
        });

        GetGUI().GetEngine().events.on_window_resized.AddListener("doge_gui_window_" + GetID(),
        [&](const event::Size& event)
        {
            UpdateContainerArea();
        });

        camera.GetEntity().OnComponentRemoval<doge::Camera>([&]()
        {
            GetGUI().GetEngine().events.on_window_resized.RemoveListener("doge_gui_window_" + GetID());
            GetGUI().RemoveElements(GetWindowCameraID());
        });

        // image
        auto& image = GetGUI().AddElement<gui::NSImage>(GetImageElementID(), GetCameraID());
        image.GetEntity().SetParent(GetEntity());
        image.SetCursorDetectable(false);
        image.SetTextureID("doge_gui_window");

        SetSize(DefaultSize);
    }

    std::string Window::GetWindowCameraID() const
    {
        return "doge_gui_window_" + GetID();
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

    void Window::OnLayerUpdated()
    {
        GetImage().SetLayer(GetLayer());
        GetGUI().GetCamera(GetWindowCameraID()).SetLayer(Layer::CreateRange(GetLayer() + 2, GetLayer() + 4));
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
        GetImage().SetAlign(GetAlign());
        UpdateContainerArea();
    }

    void Window::UpdateContainerArea()
    {
        auto& cam_comp = GetGUI().GetCameraComponent(GetWindowCameraID());
        cam_comp.size = GetSize() - GetBorderThickness().GetPosition() - GetBorderThickness().GetSize();
        cam_comp.port.SetPosition(Vec2f(0.5, 0.5) - (GetActualOrigin() - GetBorderThickness().GetPosition() - GetPosition()) / GetCameraComponent().size);
        cam_comp.port.SetSize(cam_comp.size / GetCameraComponent().size);
    }
}