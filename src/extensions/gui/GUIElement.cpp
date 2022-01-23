#include <doge/extensions/gui/GUIElement.hpp>

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

    void GUIElement::SetOwnerCamera(const std::string& owner_camera)
    {
        this->owner_camera = owner_camera;
    }

    const std::string& GUIElement::GetOwnerCamera() const
    {
        return owner_camera;
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
}