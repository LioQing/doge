#include <doge/extensions/gui/Camera.hpp>

#include <doge/extensions/gui/GUI.hpp>
#include <doge/extensions/gui/Align.hpp>

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
        this->position = position;
        UpdatePort();
    }

    const Vec2f& Camera::GetPosition() const
    {
        return position;
    }

    void Camera::SetSize(const Vec2f& size)
    {
        GetEntity().GetComponent<doge::Camera>().size = size;
        UpdatePort();
    }

    const Vec2f& Camera::GetSize() const
    {
        return GetEntity().GetComponent<doge::Camera>().size;
    }

    void Camera::SetOrigin(const Vec2f& origin)
    {
        this->origin = origin;
        UpdatePort();
    }

    const Vec2f& Camera::GetOrigin() const
    {
        return origin;
    }

    void Camera::SetAlign(std::uint8_t align)
    {
        this->align = Vec2f(.5f, .5f);

        if ((align & Align::Left) == Align::Left)
            this->align.x = 0.f;
        else if ((align & Align::Right) == Align::Right)
            this->align.x = 1.f;
        
        if ((align & Align::Top) == Align::Top)
            this->align.y = 0.f;
        else if ((align & Align::Bottom) == Align::Bottom)
            this->align.y = 1.f;

        UpdatePort();
    }

    void Camera::SetAlign(const Vec2f& align)
    {
        this->align = align;
        UpdatePort();
    }

    const Vec2f& Camera::GetAlign() const
    {
        return align;
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
                UpdatePort();
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

    Vec2f Camera::GetActualOrigin() const
    {
        return GetOrigin() + GetAlign() * GetSize();
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

    void Camera::UpdatePort()
    {
        GetEntity().GetComponent<doge::Camera>().port.SetPosition(Vec2f(0.5, 0.5) + (GetPosition() - GetActualOrigin()) / GetGUI().GetEngine().window.window_io.GetSize());
        GetEntity().GetComponent<doge::Camera>().port.SetSize(GetSize() / GetGUI().GetEngine().window.window_io.GetSize());
    }
}