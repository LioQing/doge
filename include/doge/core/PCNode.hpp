#pragma once

#include <unordered_set>
#include <memory>

#include "../utils.hpp"

namespace doge
{
    struct PCNode
    {
        static std::shared_ptr<PCNode> AddNode(lic::EntityID eid);

        PCNode* parent = nullptr;
        lic::EntityID id;
        std::unordered_set<std::shared_ptr<PCNode>> children;
        static PCNode root;

        bool HasDescendent(lic::EntityID id) const;
        const std::shared_ptr<PCNode>& GetDescendent(lic::EntityID id) const;

        bool HasParent() const;
        void SetParent(lic::EntityID id);
        void RemoveParent();

        void RemoveChild(lic::EntityID id);

        // ~PCNode()
        // {
        //     std::cout << id << std::endl;
        // }

    private:

        PCNode() = default;
    };
}
