#pragma once

#include <unordered_set>
#include <memory>

#include "../utils.hpp"

namespace doge
{
    struct PCNode
    {
        std::shared_ptr<PCNode> parent = nullptr;
        lic::EntityID eid;
        std::unordered_set<PCNode> children;

        bool HasDescendent(lic::EntityID id) const;
        std::shared_ptr<PCNode> GetDescendent(lic::EntityID id) const;
    };
}
