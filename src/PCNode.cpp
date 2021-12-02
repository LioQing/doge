#include <doge/core/PCNode.hpp>

#include <deque>

namespace doge
{
    PCNode PCNode::root;

    std::shared_ptr<PCNode> PCNode::AddNode(lic::EntityID eid)
    {
        auto node = std::shared_ptr<PCNode>(new PCNode());
        node->id = eid;
        node->parent = &root;
        root.children.insert(node);
        return node;
    }

    bool PCNode::HasDescendent(lic::EntityID eid) const
    {
        std::deque<std::shared_ptr<PCNode>> descendents;
        
        for (auto& child : children)
        {
            descendents.push_back(child);
        }

        while (!descendents.empty())
        {
            auto descendent = descendents.front();
            if (descendent->id == eid)
                return true;

            for (auto& child_of_descendent : descendent->children)
            {
                descendents.push_back(child_of_descendent);
            }
            
            descendents.pop_front();
        }

        return false;
    }

    const std::shared_ptr<PCNode>& PCNode::GetDescendent(lic::EntityID eid) const
    {
        std::deque<std::shared_ptr<PCNode>> descendents;
        
        for (auto& child : children)
        {
            descendents.push_back(child);
        }

        while (!descendents.empty())
        {
            auto descendent = descendents.front();
            if (descendent->id == eid)
                return *descendent->parent->children.find(descendent);

            for (auto& child_of_descendent : descendent->children)
            {
                descendents.push_back(child_of_descendent);
            }
            
            descendents.pop_front();
        }

        throw std::invalid_argument(std::to_string(eid) + " not found in parent-child node of " + std::to_string(id) + ".");
    }

    bool PCNode::HasParent() const
    {
        return this->parent != &root;
    }

    void PCNode::SetParent(lic::EntityID eid)
    {
        auto parent_node = root.GetDescendent(eid);
        if (this->parent)
            this->parent->RemoveChild(this->id);
        this->parent = parent_node.get();
        parent_node->children.insert(std::shared_ptr<PCNode>(this));
    }

    void PCNode::RemoveParent()
    {
        if (!HasParent())
            return;

        if (this->parent)
            this->parent->RemoveChild(id);
        this->parent = &root;
        root.children.insert(std::shared_ptr<PCNode>(this));
    }

    void PCNode::RemoveChild(lic::EntityID eid)
    {
        std::erase_if(children,
            [&](const std::shared_ptr<PCNode>& child)
            { return child->id == eid; }
        );
    }
}