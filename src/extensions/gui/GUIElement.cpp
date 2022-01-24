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

    void GUIElement::SetCameraID(const std::string& camera)
    {
        this->camera = camera;
        OnCameraUpdated();
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

    void GUIElement::SetIsScaleVec(ScaleVec is_scale_vec)
    {
        this->is_scale_vec = is_scale_vec;
        OnIsScaleVecUpdated();
    }

    GUIElement::ScaleVec GUIElement::IsScaleVec() const
    {
        return is_scale_vec;
    }

    bool GUIElement::IsScaleVec(ScaleVec scale_vec) const
    {
        return is_scale_vec & scale_vec;
    }

    Rectf GUIElement::GetRectangle() const
    {
        return Rectf(position - origin - size / 2.f, size);
    }
}