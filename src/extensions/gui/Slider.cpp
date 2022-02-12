#include <doge/extensions/gui/Slider.hpp>

#include <doge/extensions/gui/GUI.hpp>
#include <doge/extensions/gui/Draggable.hpp>
#include <doge/extensions/gui/Image.hpp>
#include <doge/extensions/gui/NSImage.hpp>

namespace doge::gui
{
    const Vec2f Slider::DefaultSize = Vec2f(200, 32);
    const Vec2f Slider::DefaultTrackSize = Vec2f(167, 20.0001);
    const float Slider::DefaultTrackLength = 150.f;

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

        draggable.on_drag_began += [&](const Vec2f& position)
        {
            if (GetDirection() == Direction::Horizontal)
                SetValue(((position.x - GetPosition().x + GetOrigin().x + (GetAlign().x - .5f) * (GetSize().x - GetTrackLength())) / GetTrackLength() + GetAlign().x) * (GetMaxValue() - GetMinValue()) + GetMinValue());
            else
                SetValue(((position.y - GetPosition().y + GetOrigin().y + (GetAlign().y - .5f) * (GetSize().y - GetTrackLength())) / GetTrackLength() + GetAlign().y) * (GetMaxValue() - GetMinValue()) + GetMinValue());
        };

        draggable.on_dragged += [&](const Vec2f& position)
        {
            if (GetDirection() == Direction::Horizontal)
                SetValue(((position.x - GetPosition().x + GetOrigin().x + (GetAlign().x - .5f) * (GetSize().x - GetTrackLength())) / GetTrackLength() + GetAlign().x) * (GetMaxValue() - GetMinValue()) + GetMinValue());
            else
                SetValue(((position.y - GetPosition().y + GetOrigin().y + (GetAlign().y - .5f) * (GetSize().y - GetTrackLength())) / GetTrackLength() + GetAlign().y) * (GetMaxValue() - GetMinValue()) + GetMinValue());
        };

        SetAlign(Align::Center);
        SetSize(DefaultSize);
        SetTrackLength(DefaultTrackLength);
        SetValue(0);
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
        auto was = this->value;

        if (value > max_value)
            value = max_value;
        else if (value < min_value)
            value = min_value;
        
        this->value = IsInt() ? std::round(value) : value;
        
        if (GetDirection() == Direction::Horizontal)
            GetThumbImage().SetPosition(Vec2f((GetValue() / (GetMaxValue() - GetMinValue()) - GetAlign().x) * GetTrackLength() - (GetAlign().x - .5f) * (GetSize().x - GetTrackLength()), -GetSize().y * (GetAlign().y - .5f)));
        else
            GetThumbImage().SetPosition(Vec2f(-GetSize().x * (GetAlign().x - .5f), (GetValue() / (GetMaxValue() - GetMinValue()) - GetAlign().y) * GetTrackLength() - (GetAlign().y - .5f) * (GetSize().y - GetTrackLength())));

        if (was != GetValue())
            on_value_changed(GetValue());
    }

    float Slider::GetValue() const
    {
        return value;
    }

    void Slider::SetTrackLength(float track_length)
    {
        this->track_length = track_length;
        SetValue(GetValue());
    }

    float Slider::GetTrackLength() const
    {
        return this->track_length;
    }

    void Slider::SetInt(bool is_int)
    {
        this->is_int = is_int;
        SetValue(GetValue());
    }

    bool Slider::IsInt() const
    {
        return is_int;
    }

    void Slider::SetDirection(Direction direction)
    {
        this->direction = direction;
        SetValue(GetValue());
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
        SetValue(GetValue());
    }

    void Slider::OnOriginUpdated()
    {
        GetDraggable().SetOrigin(GetOrigin());
        GetDraggable().SetAlign(GetAlign());

        GetTrackImage().SetOrigin(GetOrigin() + (GetAlign() - Vec2f(.5f, .5f)) * GetSize());
        GetThumbImage().SetOrigin(GetOrigin());
        SetValue(GetValue());
    }

    void Slider::OnColorUpdated()
    {
        GetTrackImage().SetColor(GetColor());
        GetThumbImage().SetColor(GetColor());
    }
}