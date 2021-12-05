#pragma once

#include <vector>

#include "../utils.hpp"

namespace doge
{
    struct Entity : public lic::Entity
    {
        Entity(EntityID id);

        // void SetParent(EntityID id) const;
        // void RemoveParent(EntityID id) const;
        // Entity GetParent() const;
        // bool HasParent() const;

        operator EntityID() const;
    };
}