#include <doge/core/Entity.hpp>

#include <stdexcept>
#include <doge/core/Engine.hpp>

namespace doge
{
    Entity::Entity(EntityID id, PCNode* pcnode) : lic::Entity(lic::Entity(id)), pcnode(pcnode)
    {
    }

    Entity Entity::GetParent() const
    {
        auto node = pcnode;
        if (!node->HasParent())
            throw std::invalid_argument(std::string("Entity ") + std::to_string(id) + " has no parent.");

        return Entity(node->parent->id, node->parent);
    }

    bool Entity::HasParent() const
    {
        return pcnode->HasParent();
    }

    bool Entity::IsParent(EntityID parent) const
    {
        return pcnode->IsParent(parent);
    }
    
    void Entity::SetParent(EntityID parent) const
    {
        pcnode->SetParent(parent);
    }
    
    void Entity::RemoveParent() const
    {
        pcnode->RemoveParent();
    }

    bool Entity::HasChild(EntityID child) const
    {
        return pcnode->HasChild(child);
    }

    std::vector<Entity> Entity::GetChildren() const
    {
        auto children_nodes = pcnode->GetChildren();
        std::vector<Entity> children;
        for (auto& child_node : children_nodes)
            children.push_back(Entity(child_node->id, child_node.get()));
        return children;
    }

    Entity::operator EntityID() const 
    {
        return lic::Entity::id;
    }
}