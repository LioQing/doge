#include <doge/extensions/gui/Element.hpp>

#include <doge/extensions/gui/GUI.hpp>
#include <doge/extensions/gui/Camera.hpp>
#include <doge/core/Entity.hpp>

namespace doge::gui
{
    Element::~Element()
    {
    }

    bool Element::TestPoint(const Vec2f& point) const
    {
        return math::TestPoint(point, GetGlobalRectangle());
    }

    void Element::ElementInitialize()
    {
        GetEntity().AddComponent<Layer>();
        GetEntity().AddComponent<Position>();

        Initialize();
        
        SetLayer(GetCamera().GetLayer().GetFirst());
    }

    GUI& Element::GetGUI() const
    {
        return *gui;
    }

    Entity Element::GetEntity() const
    {
        return GetGUI().GetElementEntity(GetID());
    }

    void Element::SetLayer(std::int32_t layer)
    {
        GetEntity().GetComponent<Layer>().layers = std::set<std::int32_t>{ layer };
        OnLayerUpdated();
    }

    std::int32_t Element::GetLayer() const
    {
        return GetEntity().GetComponent<Layer>().layers.begin().operator*();
    }

    const std::string& Element::GetID() const
    {
        return id;
    }

    const std::string& Element::GetCameraID() const
    {
        return camera;
    }

    Camera& Element::GetCamera() const
    {
        return GetGUI().GetCamera(camera);
    }

    doge::Component<doge::Camera>& Element::GetCameraComponent() const
    {
        return GetGUI().GetCameraComponent(camera);
    }

    void Element::SetSize(const Vec2f& size)
    {
        this->size = size;
        OnSizeUpdated();
        OnOriginUpdated();
    }

    const Vec2f& Element::GetSize() const
    {
        return size;
    }

    void Element::SetPosition(const Vec2f& position)
    {
        GetEntity().GetComponent<Position>().position = position;
        OnPositionUpdated();
    }

    const Vec2f& Element::GetPosition() const
    {
        return GetEntity().GetComponent<Position>().position;
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

    void Element::SetColor(const Color& color)
    {
        this->color = color;
        OnColorUpdated();
    }

    const Color& Element::GetColor() const
    {
        return color;
    }

    void Element::SetAlign(std::uint8_t align)
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
        
        OnOriginUpdated();
    }

    void Element::SetAlign(const Vec2f& align)
    {
        this->align = align;
        OnOriginUpdated();
    }

    const Vec2f& Element::GetAlign() const
    {
        return align;
    }

    Rectf Element::GetLocalRectangle() const
    {
        return Rectf(GetPosition() - GetActualOrigin(), GetSize());
    }

    Rectf Element::GetGlobalRectangle() const
    {
        return Rectf(global::GetPosition(GetEntity()) - GetActualOrigin(), GetSize());
    }

    Vec2f Element::GetActualOrigin() const
    {
        return GetOrigin() + GetAlign() * GetSize();
    }
}