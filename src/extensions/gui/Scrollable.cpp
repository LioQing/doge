#include <doge/extensions/gui/Scrollable.hpp>

#include <doge/extensions/gui/GUI.hpp>
#include <doge/extensions/gui/Camera.hpp>
#include <doge/extensions/gui/Draggable.hpp>

namespace doge::gui
{
    Scrollable::~Scrollable()
    {
    }

    void Scrollable::Initialize()
    {
        scrollable_camera = GetCameraID();
        get_camera_to_be_focused = [this]() -> Camera& { return this->GetScrollableCamera(); };

        GetScrollableCamera().on_scrolled.AddListener("doge_gui_scrollable_" + GetID(), 
        [&](const Vec2f& position, io::Mouse::Wheel wheel, float delta)
        { this->OnCameraScrolled(position, wheel, delta); });
    }

    void Scrollable::SetScrollableCamera(const std::string& id)
    {
        auto was = this->scrollable_camera;
        this->scrollable_camera = id;

        if (was != id)
        {
            GetGUI().GetCamera(was).on_scrolled.RemoveListener("doge_gui_scrollable_" + GetID());
            GetScrollableCamera().on_scrolled.AddListener("doge_gui_scrollable_" + GetID(), 
            [&](const Vec2f& position, io::Mouse::Wheel wheel, float delta)
            { this->OnCameraScrolled(position, wheel, delta); });
        }
    }

    const std::string& Scrollable::GetScrollableCameraID() const
    {
        return scrollable_camera;
    }

    Camera& Scrollable::GetScrollableCamera() const
    {
        return GetGUI().GetCamera(GetScrollableCameraID());
    }

    void Scrollable::SetArea(const Rectf& area)
    {
        this->area = area;
    }

    const Rectf& Scrollable::GetArea() const
    {
        return area;
    }

    void Scrollable::SetAreaUnlimited(bool unlimited)
    {
        this->area_unlimited = unlimited;
    }

    bool Scrollable::IsAreaUnlimited() const
    {
        return area_unlimited;
    }

    void Scrollable::SetScrollSpeed(const Vec2f& scroll_speed)
    {
        this->scroll_speed = scroll_speed;
    }

    void Scrollable::SetScrollSpeed(float scroll_speed)
    {
        this->scroll_speed = scroll_speed * Vec2f::One;
    }

    const Vec2f& Scrollable::GetScrollSpeed() const
    {
        return scroll_speed;
    }

    std::string Scrollable::GetHorizontalScrollBarElementID() const
    {
        return "doge_gui_scrollable_" + GetID() + "_draggable_horizontal";
    }

    std::string Scrollable::GetVerticalScrollBarElementID() const
    {
        return "doge_gui_scrollable_" + GetID() + "_draggable_vertical";
    }

    Draggable& Scrollable::GetHorizontalScrollBar() const
    {
        return static_cast<Draggable&>(GetGUI().GetElement(GetHorizontalScrollBarElementID()));
    }

    Draggable& Scrollable::GetVerticalScrollBar() const
    {
        return static_cast<Draggable&>(GetGUI().GetElement(GetVerticalScrollBarElementID()));
    }

    void Scrollable::OnSizeUpdated()
    {
        Bound();
    }

    void Scrollable::Bound()
    {
        if (IsAreaUnlimited())
            return;

        auto& cam_pos = GetScrollableCamera().GetCameraPosition();

        if (cam_pos.y < area.top)
            GetScrollableCamera().SetCameraPosition(Vec2f(cam_pos.x, area.top));
        else if (cam_pos.y > area.top + area.height)
            GetScrollableCamera().SetCameraPosition(Vec2f(cam_pos.x, area.top + area.height));

        if (cam_pos.x < area.left)
            GetScrollableCamera().SetCameraPosition(Vec2f(area.left, cam_pos.y));
        else if (cam_pos.x > area.left + area.width)
            GetScrollableCamera().SetCameraPosition(Vec2f(area.left + area.width, cam_pos.y));
    }

    void Scrollable::OnCameraScrolled(const Vec2f& position, io::Mouse::Wheel wheel, float delta)
    {
        if (wheel == io::Mouse::Wheel::Vertical)
        {
            GetScrollableCamera().SetCameraPosition(GetScrollableCamera().GetCameraPosition() - delta * Vec2f(0, scroll_speed.y));
        }
        else
        {
            GetScrollableCamera().SetCameraPosition(GetScrollableCamera().GetCameraPosition() - delta * Vec2f(scroll_speed.x, 0));
        }

        Bound();
    }
}