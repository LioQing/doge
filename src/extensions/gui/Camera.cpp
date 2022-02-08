#include <doge/extensions/gui/Camera.hpp>

#include <doge/extensions/gui/GUI.hpp>

namespace doge::gui
{
    GUI& Camera::GetGUI() const
    {
        return *gui;
    }

    Entity Camera::GetEntity() const
    {
        return GetGUI().GetCameraEntity(GetID());
    }

    void Camera::SetLayer(const Layer& layer)
    {
        GetLayer() = layer;
    }

    bool Camera::HasLayer(std::int32_t layer) const
    {
        return GetEntity().GetComponent<Layer>().layers.contains(layer);
    }

    Layer& Camera::GetLayer() const
    {
        return GetEntity().GetComponent<Layer>();
    }

    const std::string& Camera::GetID() const
    {
        return id;
    }

    void Camera::SetPosition(const Vec2f& position)
    {
        GetEntity().GetComponent<Position>().position = position;
    }

    const Vec2f& Camera::GetPosition() const
    {
        return GetEntity().GetComponent<Position>().position;
    }

    void Camera::SetSize(const Vec2f& size)
    {
        GetEntity().GetComponent<doge::Camera>().size = size;
    }

    const Vec2f& Camera::GetSize() const
    {
        return GetEntity().GetComponent<doge::Camera>().size;
    }

    void Camera::SetAbsolutePort(bool is_absolute_port)
    {
        this->is_absolute_port = is_absolute_port;

        GetGUI().GetEngine().events.on_window_resized.RemoveListener("doge_gui_camera_" + id);
        if (is_absolute_port)
        {
            GetGUI().GetEngine().events.on_window_resized.AddListener("doge_gui_camera_" + id,
            [&](const event::Size& event)
            {
                GetEntity().GetComponent<doge::Camera>().port.SetSize(GetSize() / event.size);
            });
        }
        else
        {
            GetGUI().GetEngine().events.on_window_resized.AddListener("doge_gui_camera_" + id,
            [&](const event::Size& event)
            {
                SetSize(event.size * GetEntity().GetComponent<doge::Camera>().port.GetSize());
            });
        }
    }

    bool Camera::IsAbsolutePort() const
    {
        return is_absolute_port;
    }

    void Camera::Initialize()
    {
        GetEntity().AddComponent<doge::Camera>(GetGUI().GetEngine().window.window_io.GetSize());
        GetEntity().AddComponent<Layer>();
        GetEntity().AddComponent<Position>();

        GetEntity().OnComponentRemoval<EntityInfo>([&]()
        {
            GetGUI().GetEngine().events.on_window_resized.RemoveListener("doge_gui_camera_" + id);
        });

        SetAbsolutePort(false);
    }
}