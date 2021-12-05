#pragma once

#include <unordered_set>
#include <vector>
#include <memory>

#include "../utils.hpp"

namespace doge
{
    struct PCNode
    {
        static const std::shared_ptr<PCNode> AddNode(EntityID eid);

        PCNode* parent = nullptr;
        EntityID id;
        std::unordered_set<std::shared_ptr<PCNode>> children;
        static PCNode root;

        bool HasDescendent(EntityID eid) const;
        const std::shared_ptr<PCNode> GetDescendent(EntityID eid) const;
        const std::vector<std::shared_ptr<PCNode>> GetDescendents() const;

        bool HasParent() const;
        bool IsParent(EntityID eid) const;
        void SetParent(EntityID eid);
        void RemoveParent();

        bool HasChild(EntityID eid) const;
        const std::shared_ptr<PCNode> GetChild(EntityID eid) const;
        const std::vector<std::shared_ptr<PCNode>> GetChildren() const;
        void RemoveChild(EntityID eid);

    private:

        PCNode() = default;
    };
}
