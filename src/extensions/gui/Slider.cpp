#include <doge/extensions/gui/Slider.hpp>

#include <doge/extensions/gui/GUI.hpp>
#include <doge/extensions/gui/Draggable.hpp>
#include <doge/extensions/gui/Image.hpp>
#include <doge/extensions/gui/NSImage.hpp>

namespace doge::gui
{
    const Vec2f Slider::DefaultSize = Vec2f(200, 32);
    const Vec2f Slider::DefaultTrackSize = Vec2f(200, 20.0001);

    Slider::~Slider()
    {        
    }

    void Slider::Initialize()
    {
        auto& draggable = GetGUI().AddElement<Draggable>(GetDraggableElementID(), GetCameraID());
        draggable.GetEntity().SetParent(GetEntity());

        auto& track_image = GetGUI().AddElement<NSImage>(GetTrackImageElementID(), GetCameraID());
        track_image.GetEntity().SetParent(GetEntity());
        track_image.SetTextureID("doge_gui_slider_track");
        track_image.SetSize(DefaultTrackSize);

        auto& thumb_image = GetGUI().AddElement<Image>(GetThumbImageElementID(), GetCameraID());
        thumb_image.GetEntity().SetParent(GetEntity());
        thumb_image.SetTextureID("doge_gui_slider_thumb");
        thumb_image.SetSize(Vec2f(32, 32));
        thumb_image.SetAlign(Align::Center);

        draggable.on_drag_began += [&](const Vec2f& position)
        {
            if (GetDirection() == Direction::Horizontal)
                SetValue(position.x - GetPosition().x);
            else
                SetValue(position.y - GetPosition().y);
        };

        draggable.on_dragged += [&](const Vec2f& position)
        {
            if (GetDirection() == Direction::Horizontal)
                SetValue(position.x - GetPosition().x);
            else
                SetValue(position.y - GetPosition().y);
        };

        SetAlign(Align::Center);
        SetSize(DefaultSize);
    }

    void Slider::SetMinValue(float min_value)
    {
        this->min_value = min_value;
    }

    float Slider::GetMinValue() const
    {
        return min_value;
    }

    void Slider::SetMaxValue(float max_value)
    {
        this->max_value = max_value;
    }

    float Slider::GetMaxValue() const
    {
        return max_value;
    }

    void Slider::SetValue(float value)
    {
        this->value = value;

        if (GetDirection() == Direction::Horizontal)
        {
            if (value > GetSize().x / 2.f)
                value = GetSize().x / 2.f;
            else if (value < -GetSize().x / 2.f)
                value = -GetSize().x / 2.f;
                
            GetThumbImage().SetPosition(Vec2f(value, 0));
        }
        else
        {
            if (value > GetSize().y / 2.f)
                value = GetSize().y / 2.f;
            else if (value < -GetSize().y / 2.f)
                value = -GetSize().y / 2.f;

            GetThumbImage().SetPosition(Vec2f(0, value));
        }

        on_value_changed(value);
    }

    float Slider::GetValue() const
    {
        return value;
    }

    void Slider::SetInt(bool is_int)
    {
        this->is_int = is_int;
    }

    bool Slider::IsInt() const
    {
        return is_int;
    }

    void Slider::SetDirection(Direction direction)
    {
        this->direction = direction;
    }

    Slider::Direction Slider::GetDirection() const
    {
        return direction;
    }
    
    std::string Slider::GetDraggableElementID() const
    {
        return "doge_gui_slider_" + GetID() + "_draggable";
    }

    Draggable& Slider::GetDraggable() const
    {
        return static_cast<Draggable&>(GetGUI().GetElement(GetDraggableElementID()));
    }

    std::string Slider::GetTrackImageElementID() const
    {
        return "doge_gui_slider_" + GetID() + "_image_track";
    }

    NSImage& Slider::GetTrackImage() const
    {
        return static_cast<NSImage&>(GetGUI().GetElement(GetTrackImageElementID()));
    }

    std::string Slider::GetThumbImageElementID() const
    {
        return "doge_gui_slider_" + GetID() + "_image_thumb";
    }

    Image& Slider::GetThumbImage() const
    {
        return static_cast<Image&>(GetGUI().GetElement(GetThumbImageElementID()));
    }

    void Slider::OnLayerUpdated()
    {
        GetDraggable().SetLayer(GetLayer() + 1);
        GetTrackImage().SetLayer(GetLayer());
        GetThumbImage().SetLayer(GetLayer() + 1);
    }

    void Slider::OnSizeUpdated()
    {
        GetDraggable().SetSize(GetSize());
    }

    void Slider::OnOriginUpdated()
    {
        GetDraggable().SetOrigin(GetOrigin());
        GetDraggable().SetAlign(GetAlign());
    }

    void Slider::OnColorUpdated()
    {
        GetTrackImage().SetColor(GetColor());
        GetThumbImage().SetColor(GetColor());
    }
}