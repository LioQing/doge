#include <doge/extensions/gui/GUIElement.hpp>

#include <doge/extensions/gui/gui.hpp>

namespace doge
{
    GUIElement::~GUIElement()
    {
    };

    void GUIElement::SetID(const std::string& id)
    {
        this->id = id;
    }

    const std::string& GUIElement::GetID() const
    {
        return id;
    }

    void GUIElement::SetTextureID(const std::string& texture_id)
    {
        this->texture_id = texture_id;
    }

    const std::string& GUIElement::GetTextureID() const
    {
        return texture_id;
    }

    void GUIElement::SetCamera(const std::string& owner_camera)
    {
        this->camera = owner_camera;
    }

    const std::string& GUIElement::GetCamera() const
    {
        return camera;
    }

    doge::Component<Camera>& GUIElement::GetCameraComponent() const
    {
        return gui::GetCamera(camera);
    }

    void GUIElement::SetSize(const Vec2f& size)
    {
        this->size = size;
    }

    const Vec2f& GUIElement::GetSize() const
    {
        return size;
    }

    void GUIElement::SetPosition(const Vec2f& position)
    {
        this->position = position;
    }

    const Vec2f& GUIElement::GetPosition() const
    {
        return position;
    }

    void GUIElement::SetIsScaleVec(ScaleVec is_scale_vec)
    {
        this->is_scale_vec = is_scale_vec;
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