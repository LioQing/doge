#include <doge/extensions/gui/Text.hpp>

#include <doge/extensions/gui/GUI.hpp>

namespace doge::gui
{
    void Text::Initialize()
    {
        GetEntity().AddComponent(Layer::Create(GetLayer() + 1));
        GetEntity().AddComponent(doge::Text
        {
            .font_id = "",
            .string = U"Text",
            .align = doge::Text::Align::Left,
        });
        GetEntity().AddComponent<Position>(0, 0);
    }

    std::int32_t Text::GetLayer() const
    {
        return Element::GetLayer() + 1;
    }

    void Text::SetString(const std::u32string& str)
    {
        GetEntity().GetComponent<doge::Text>().string = str;
        UpdateVerticalOriginOffset();
    }

    const std::u32string& Text::GetString() const
    {
        return GetEntity().GetComponent<doge::Text>().string;
    }

    void Text::SetFont(const std::string& font_id)
    {
        GetEntity().GetComponent<doge::Text>().font_id = font_id;
        UpdateVerticalOriginOffset();
    }

    const std::string& Text::GetTextFontID() const
    {
        return GetEntity().GetComponent<doge::Text>().font_id;
    }

    void Text::SetFontSize(std::uint32_t font_size)
    {
        GetEntity().GetComponent<doge::Text>().font_size = font_size;
        UpdateVerticalOriginOffset();
    }

    std::uint32_t Text::GetFontSize() const
    {
        return GetEntity().GetComponent<doge::Text>().font_size;
    }

    void Text::SetLineSpacingFactor(float factor)
    {
        GetEntity().GetComponent<doge::Text>().line_spacing_factor = factor;
        UpdateVerticalOriginOffset();
    }

    float Text::GetLineSpacingFactor() const
    {
        return GetEntity().GetComponent<doge::Text>().line_spacing_factor;
    }

    void Text::SetAlign(doge::Text::Align align)
    {
        GetEntity().GetComponent<doge::Text>().align = align;
        UpdateVerticalOriginOffset();
    }

    doge::Text::Align Text::GetAlign() const
    {
        return GetEntity().GetComponent<doge::Text>().align;
    }

    void Text::SetVerticalAlign(Text::VerticalAlign vertical_align)
    {
        this->vertical_align = vertical_align;
        UpdateVerticalOriginOffset();
    }

    Text::VerticalAlign Text::GetVerticalAlign() const
    {
        return vertical_align;
    }

    void Text::SetAppearance(const doge::Text::Appearance& appear, std::size_t pos)
    {
        GetEntity().GetComponent<doge::Text>().character_appearances[pos] = appear;
        UpdateVerticalOriginOffset();
    }

    const doge::Text::Appearance& Text::GetAppearance(std::size_t pos) const
    {
        if (GetEntity().GetComponent<doge::Text>().character_appearances.contains(pos))
            return GetEntity().GetComponent<doge::Text>().character_appearances.at(pos);
        
        return std::prev(GetEntity().GetComponent<doge::Text>().character_appearances.lower_bound(pos))->second;
    }

    void Text::OnPositionUpdated()
    {
        GetEntity().GetComponent<Position>().position = GetPosition();
    }

    void Text::OnOriginUpdated()
    {
        UpdateVerticalOriginOffset();
        GetEntity().GetComponent<doge::Text>().origin = GetOrigin() + vertical_origin_offset * doge::Vec2f::j;
    }

    void Text::UpdateVerticalOriginOffset()
    {
        if (vertical_align == VerticalAlign::Top)
        {
            vertical_origin_offset = 0.f;
            GetEntity().GetComponent<doge::Text>().origin = GetOrigin() + vertical_origin_offset * doge::Vec2f::j;
            return;
        }

        auto& text = GetEntity().GetComponent<doge::Text>();
        if (text.font_id == "")
        {
            vertical_origin_offset = 0.f;
            GetEntity().GetComponent<doge::Text>().origin = GetOrigin() + vertical_origin_offset * doge::Vec2f::j;
            return;
        }

        auto height = GetGUI().GetEngine().assets.GetFont(text.font_id).GetLineSpacing(text.font_size) * text.line_spacing_factor;
        auto line = std::count(text.string.begin(), text.string.end(), U'\n') + 1;

        if (vertical_align == VerticalAlign::Center)
            vertical_origin_offset = line * height / 2.f;
        else
            vertical_origin_offset = line * height;

        GetEntity().GetComponent<doge::Text>().origin = GetOrigin() + vertical_origin_offset * doge::Vec2f::j;
    }
}