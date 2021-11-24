#pragma once

#include <vector>

#include "../utils.hpp"

namespace doge
{
    struct Entity : public lic::Entity
    {
        lic::EntityID parent;
        std::vector<lic::EntityID> children;
        bool has_parent = false;
    };
}