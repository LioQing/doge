#include <doge/extensions/gui/Text.hpp>

#include <doge/extensions/gui/GUI.hpp>

namespace doge::gui
{
    void Text::Initialize()
    {
        GetEntity().AddComponent(Layer::Create(GetLayer()));
        GetEntity().AddComponent<Position>(0, 0);
        GetEntity().AddComponent<doge::Text>();

        SetString(U"Text");
    }

    std::int32_t Text::GetLayer() const
    {
        return Element::GetLayer() + 1;
    }

    void Text::SetString(const std::u32string& str)
    {
        GetEntity().GetComponent<doge::Text>().string = str;
        OnOriginUpdated();
    }

    const std::u32string& Text::GetString() const
    {
        return GetEntity().GetComponent<doge::Text>().string;
    }

    void Text::SetFont(const std::string& font_id)
    {
        GetEntity().GetComponent<doge::Text>().font_id = font_id;
        OnOriginUpdated();
    }

    const std::string& Text::GetTextFontID() const
    {
        return GetEntity().GetComponent<doge::Text>().font_id;
    }

    void Text::SetFontSize(std::uint32_t font_size)
    {
        GetEntity().GetComponent<doge::Text>().font_size = font_size;
        OnOriginUpdated();
    }

    std::uint32_t Text::GetFontSize() const
    {
        return GetEntity().GetComponent<doge::Text>().font_size;
    }

    void Text::SetLineSpacingFactor(float factor)
    {
        GetEntity().GetComponent<doge::Text>().line_spacing_factor = factor;
        OnOriginUpdated();
    }

    float Text::GetLineSpacingFactor() const
    {
        return GetEntity().GetComponent<doge::Text>().line_spacing_factor;
    }

    void Text::SetAlign(doge::Text::Align align)
    {
        GetEntity().GetComponent<doge::Text>().align = align;
        OnOriginUpdated();
    }

    doge::Text::Align Text::GetAlign() const
    {
        return GetEntity().GetComponent<doge::Text>().align;
    }

    void Text::SetVerticalAlign(Text::VerticalAlign vertical_align)
    {
        this->vertical_align = vertical_align;
        OnOriginUpdated();
    }

    Text::VerticalAlign Text::GetVerticalAlign() const
    {
        return vertical_align;
    }

    void Text::SetAppearance(const doge::Text::Appearance& appear, std::size_t pos)
    {
        GetEntity().GetComponent<doge::Text>().character_appearances[pos] = appear;
        OnOriginUpdated();
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
        if (vertical_align == VerticalAlign::Top)
        {
            GetEntity().GetComponent<doge::Text>().origin = GetOrigin();
            return;
        }

        auto& text = GetEntity().GetComponent<doge::Text>();
        if (text.font_id == "")
        {
            GetEntity().GetComponent<doge::Text>().origin = GetOrigin();
            return;
        }

        auto height = GetGUI().GetEngine().assets.GetFont(text.font_id).GetLineSpacing(text.font_size) * text.line_spacing_factor;
        auto line = std::count(text.string.begin(), text.string.end(), U'\n') + 1;

        if (vertical_align == VerticalAlign::Center)
            GetEntity().GetComponent<doge::Text>().origin = GetOrigin() + line * height / 2.f * doge::Vec2f::j;
        else
            GetEntity().GetComponent<doge::Text>().origin = GetOrigin() + line * height * doge::Vec2f::j;
    }
}