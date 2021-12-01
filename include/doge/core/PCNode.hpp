#pragma once

#include <unordered_set>
#include <memory>

#include "../utils.hpp"

namespace doge
{
    struct PCNode
    {
        PCNode* parent;
        lic::EntityID eid;
        std::unordered_set<std::unique_ptr<PCNode>> children;

        bool HasDescendent(lic::EntityID id) const;
        PCNode* GetDescendent(lic::EntityID id);

        std::strong_ordering operator<=>(const PCNode& other) const;
    };
}
