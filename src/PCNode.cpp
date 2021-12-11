#include <doge/core/PCNode.hpp>

#include <deque>

namespace doge
{
    PCNode PCNode::root;

    const std::shared_ptr<PCNode> PCNode::AddNode(EntityID eid)
    {
        auto node = std::shared_ptr<PCNode>(new PCNode());
        node->id = eid;
        node->parent = &root;
        root.children.insert(node);
        return node;
    }

    bool PCNode::HasDescendent(EntityID eid) const
    {
        if (HasChild(eid))
            return true;

        std::deque<std::shared_ptr<PCNode>> descendents(children.begin(), children.end());

        while (!descendents.empty())
        {
            auto descendent = descendents.front();

            for (auto& child_of_descendent : descendent->children)
            {
                if (child_of_descendent->id == eid)
                    return true;
                descendents.push_back(child_of_descendent);
            }
            
            descendents.pop_front();
        }

        return false;
    }

    const std::shared_ptr<PCNode> PCNode::GetDescendent(lic::EntityID eid) const
    {
        if (HasChild(eid))
            return GetChild(eid);

        std::deque<std::shared_ptr<PCNode>> descendents(children.begin(), children.end());

        while (!descendents.empty())
        {
            auto descendent = descendents.front();

            for (auto& child_of_descendent : descendent->children)
            {
                if (child_of_descendent->id == eid)
                    return child_of_descendent;
                descendents.push_back(child_of_descendent);
            }
            
            descendents.pop_front();
        }

        throw std::invalid_argument(std::to_string(eid) + " not found in parent-child node of " + std::to_string(id) + ".");
    }

    const std::vector<std::shared_ptr<PCNode>> PCNode::GetDescendents() const
    {
        std::vector<std::shared_ptr<PCNode>> descendents(children.begin(), children.end());
        std::deque<std::shared_ptr<PCNode>> to_be_searched(children.begin(), children.end());

        while (!to_be_searched.empty())
        {
            auto searching = to_be_searched.front();

            for (auto& child_of_searching : searching->children)
            {
                descendents.push_back(child_of_searching);
                to_be_searched.push_back(child_of_searching);
            }

            to_be_searched.pop_front();
        }

        return descendents;
    }

    bool PCNode::HasParent() const
    {
        return this->parent != &root;
    }

    bool PCNode::IsParent(EntityID eid) const
    {
        return this->parent->id == eid;
    }

    void PCNode::SetParent(EntityID eid)
    {
        auto parent_node = root.GetDescendent(eid);
        if (this->parent)
            parent_node->children.insert(this->parent->GetChild(this->id));
            this->parent->RemoveChild(this->id);
        this->parent = parent_node.get();
    }

    void PCNode::RemoveParent()
    {
        if (!HasParent())
            return;

        if (this->parent)
            root.children.insert(this->parent->GetChild(this->id));
            this->parent->RemoveChild(id);
        this->parent = &root;
    }

    bool PCNode::HasChild(EntityID eid) const
    {
        return std::find_if(children.begin(), children.end(),
            [&](const std::shared_ptr<PCNode>& child)
            { return child->id == eid; })
            != children.end();
    }

    const std::shared_ptr<PCNode> PCNode::GetChild(EntityID eid) const
    {
        return *std::find_if(children.begin(), children.end(),
            [&](const std::shared_ptr<PCNode>& child)
            { return child->id == eid; }
        );
    }

    const std::vector<std::shared_ptr<PCNode>> PCNode::GetChildren() const
    {
        return std::vector<std::shared_ptr<PCNode>>(children.begin(), children.end());
    }

    void PCNode::RemoveChild(EntityID eid)
    {
        std::erase_if(children,
            [&](const std::shared_ptr<PCNode>& child)
            { return child->id == eid; }
        );
    }
}