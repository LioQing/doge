#include <doge/core/PCNode.hpp>

#include <deque>

namespace doge
{
    bool PCNode::HasDescendent(lic::EntityID id) const
    {
        std::deque<std::shared_ptr<PCNode>> descendents(children.begin(), children.end());
        while (!descendents.empty())
        {
            auto descendent = descendents.front();
            if (descendent->eid == id)
                return true;

            for (auto child_of_descendent : descendent->children)
            {
                descendents.emplace_back(child_of_descendent);
            }
            
            descendents.pop_front();
        }

        return false;
    }

    std::shared_ptr<PCNode> PCNode::GetDescendent(lic::EntityID id) const
    {
        if (!HasDescendent(id))
        {
            throw std::invalid_argument(std::to_string(id) + " not found in parent-child node.");
        }

        std::deque<std::shared_ptr<PCNode>> descendents(children.begin(), children.end());
        while (!descendents.empty())
        {
            auto descendent = descendents.front();
            if (descendent->eid == id)
                return descendent;

            for (auto child_of_descendent : descendent->children)
            {
                descendents.emplace_back(child_of_descendent);
            }
            
            descendents.pop_front();
        }
    }
}