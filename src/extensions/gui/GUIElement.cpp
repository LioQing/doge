#include <doge/extensions/gui/GUIElement.hpp>

#include <doge/extensions/gui/gui.hpp>
#include <doge/core/Entity.hpp>

namespace doge
{
    GUIElement::~GUIElement()
    {
    };

    Entity GUIElement::GetEntity() const
    {
        return gui::GetElementEntity(GetID());
    }

    const std::string& GUIElement::GetID() const
    {
        return id;
    }

    const std::string& GUIElement::GetCameraID() const
    {
        return camera;
    }

    doge::Component<Camera>& GUIElement::GetCameraComponent() const
    {
        return gui::GetCameraComponent(camera);
    }

    void GUIElement::SetSize(const Vec2f& size)
    {
        this->size = size;
        OnSizeUpdated();
    }

    const Vec2f& GUIElement::GetSize() const
    {
        return size;
    }

    void GUIElement::SetPosition(const Vec2f& position)
    {
        this->position = position;
        OnPositionUpdated();
    }

    const Vec2f& GUIElement::GetPosition() const
    {
        return position;
    }

    void GUIElement::SetOrigin(const Vec2f& origin)
    {
        this->origin = origin;
        OnOriginUpdated();
    }

    const Vec2f& GUIElement::GetOrigin() const
    {
        return origin;
    }

    Rectf GUIElement::GetRectangle() const
    {
        return Rectf(position - origin - size / 2.f, size);
    }
}