#include <doge/core/PCNode.hpp>

#include <deque>

namespace doge
{
    bool PCNode::HasDescendent(lic::EntityID id) const
    {
        std::deque<PCNode*> descendents(children.size());
        std::transform(children.begin(), children.end(), std::back_inserter(descendents), [](const std::unique_ptr<PCNode>& node) { return node.get(); });
        while (!descendents.empty())
        {
            auto descendent = descendents.front();
            if (descendent->eid == id)
                return true;

            for (auto& child_of_descendent : descendent->children)
            {
                descendents.emplace_back(child_of_descendent.get());
            }
            
            descendents.pop_front();
        }

        return false;
    }

    PCNode* PCNode::GetDescendent(lic::EntityID id)
    {
        std::deque<PCNode*> descendents(children.size());
        std::transform(children.begin(), children.end(), std::back_inserter(descendents), [](const std::unique_ptr<PCNode>& node) { return node.get(); });
        while (!descendents.empty())
        {
            auto descendent = descendents.front();
            if (descendent->eid == id)
                return descendent;

            for (auto& child_of_descendent : descendent->children)
            {
                descendents.emplace_back(child_of_descendent.get());
            }
            
            descendents.pop_front();
        }

        throw std::invalid_argument(std::to_string(id) + " not found in parent-child node.");
    }

    std::strong_ordering PCNode::operator<=>(const PCNode& other) const
    {
        return other.eid <=> eid;
    }
}