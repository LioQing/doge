#pragma once

#include <vector>

#include "../utils.hpp"

namespace doge
{
    struct Entity : public lic::Entity
    {
        Entity(lic::EntityID id);

        void SetParent(lic::EntityID id) const;
        void RemoveParent(lic::EntityID id) const;
        Entity GetParent() const;
        bool HasParent() const;

        operator lic::EntityID() const;
    };
}