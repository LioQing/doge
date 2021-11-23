#pragma once

#include <vector>

#include "../utils.hpp"

namespace doge
{
    struct Entity : public lic::Entity
    {
        std::vector<lic::EntityID> parents;
        std::vector<lic::EntityID> children;
    };
}