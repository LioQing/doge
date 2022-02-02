#include <doge/extensions/gui/Element.hpp>

#include <doge/extensions/gui/GUI.hpp>
#include <doge/core/Entity.hpp>

namespace doge::gui
{
    Element::~Element()
    {
    };

    std::int32_t Element::GetLayer() const
    {
        return GetGUI().GetCameraLayer(GetCameraID());
    }

    GUI& Element::GetGUI() const
    {
        return *gui;
    }

    Entity Element::GetEntity() const
    {
        return GetGUI().GetElementEntity(GetID());
    }

    const std::string& Element::GetID() const
    {
        return id;
    }

    const std::string& Element::GetCameraID() const
    {
        return camera;
    }

    doge::Component<Camera>& Element::GetCameraComponent() const
    {
        return GetGUI().GetCameraComponent(camera);
    }

    void Element::SetSize(const Vec2f& size)
    {
        this->size = size;
        OnSizeUpdated();
    }

    const Vec2f& Element::GetSize() const
    {
        return size;
    }

    void Element::SetPosition(const Vec2f& position)
    {
        this->position = position;
        OnPositionUpdated();
    }

    const Vec2f& Element::GetPosition() const
    {
        return position;
    }

    void Element::SetOrigin(const Vec2f& origin)
    {
        this->origin = origin;
        OnOriginUpdated();
    }

    const Vec2f& Element::GetOrigin() const
    {
        return origin;
    }

    Rectf Element::GetRectangle() const
    {
        return Rectf(position - origin - size / 2.f, size);
    }
}