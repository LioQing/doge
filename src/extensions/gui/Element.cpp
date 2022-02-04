#include <doge/extensions/gui/Element.hpp>

#include <doge/extensions/gui/GUI.hpp>
#include <doge/core/Entity.hpp>

namespace doge::gui
{
    Element::~Element()
    {
    }

    void Element::ElementInitialize()
    {
        GetEntity().AddComponent(Layer::Create(GetGUI().GetCameraLayer(GetCameraID())));
        GetEntity().AddComponent<Position>(0, 0);

        Initialize();
    }

    GUI& Element::GetGUI() const
    {
        return *gui;
    }

    Entity Element::GetEntity() const
    {
        return GetGUI().GetElementEntity(GetID());
    }

    void Element::SetLocalLayer(std::int32_t layer)
    {
        SetLayer(GetGUI().GetCameraLayer(GetCameraID()) + layer);
        OnLayerUpdated();
    }

    void Element::SetLayer(std::int32_t layer)
    {
        GetEntity().GetComponent<Layer>().layers = std::set<std::int32_t>{ layer };
        OnLayerUpdated();
    }

    std::int32_t Element::GetLocalLayer() const
    {
        return GetLayer() - GetGUI().GetCameraLayer(GetCameraID());
    }

    std::int32_t Element::GetLayer() const
    {
        return GetEntity().GetComponent<Layer>().layers.begin().operator*();
    }

    void Element::SetCursorDetectable(bool is_cursor_detectable)
    {
        this->is_cursor_detectable = is_cursor_detectable;
        OnCursorDetectableUpdated();
    }

    bool Element::IsCursorDetectable() const
    {
        return is_cursor_detectable;
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

    void Element::SetAlign(Align align)
    {
        if (align % Align::Base == Align::Left)
            this->align.x = -1.f;
        else if (align % Align::Base == Align::Right)
            this->align.x = 1.f;
        
        if (align / Align::Base * Align::Base == Align::Top)
            this->align.y = -1.f;
        else if (align / Align::Base * Align::Base == Align::Bottom)
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

    Rectf Element::GetRectangle() const
    {
        return Rectf(GetPosition() - GetOrigin() - GetSize() / 2.f, GetSize());
    }

    Vec2f Element::GetActualOrigin() const
    {
        return GetOrigin() + GetAlign() * GetSize();
    }
}