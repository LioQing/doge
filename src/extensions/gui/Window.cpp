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
        camera.SetAbsolutePort(true);
        camera.SetAlign(Vec2f::Zero);

        camera.GetEntity().OnComponentRemoval<doge::Camera>([&]()
        {
            GetGUI().GetEngine().events.on_window_resized.RemoveListener("doge_gui_window_" + GetID());
            GetGUI().RemoveElements(GetWindowCameraID());
        });

        GetEntity().OnComponentRemoval<ElementComponent>([&, camera_entity = camera.GetEntity()]()
        {
            GetGUI().GetEngine().DestroyEntity(camera_entity);
        });

        // image
        auto& image = GetGUI().AddElement<gui::NSImage>(GetImageElementID(), GetCameraID());
        image.GetEntity().SetParent(GetEntity());
        image.SetTextureID("doge_gui_window");

        SetSize(DefaultSize);
    }

    std::string Window::GetWindowCameraID() const
    {
        return "doge_gui_window_" + GetID() + "_camera";
    }

    Camera& Window::GetWindowCamera() const
    {
        return GetGUI().GetCamera(GetWindowCameraID());
    }

    void Window::SetBorderThickness(const Rectf& border_thickness)
    {
        this->border_thickness = border_thickness;
        OnSizeUpdated();
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
        GetWindowCamera().SetLayer(Layer::CreateRange(GetLayer() + 4, GetLayer() + 6));
    }

    void Window::OnSizeUpdated()
    {
        GetImage().SetSize(GetSize());
        GetWindowCamera().SetSize(GetSize() - GetBorderThickness().GetPosition() - GetBorderThickness().GetSize());
    }

    void Window::OnPositionUpdated()
    {
        GetWindowCamera().SetPosition(GetPosition());
    }

    void Window::OnOriginUpdated()
    {
        GetImage().SetOrigin(GetOrigin());
        GetImage().SetAlign(GetAlign());

        GetWindowCamera().SetOrigin(GetActualOrigin() - GetBorderThickness().GetPosition());
    }
}