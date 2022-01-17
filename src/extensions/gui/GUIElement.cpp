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
}