#pragma once

#include <vector>

#include "../utils.hpp"

namespace doge
{
    struct Engine;
    struct PCNode;

    struct Entity : public lic::Entity
    {
        Entity(EntityID EntityID, PCNode* pcnode);

        // const Entity GetParent(EntityID eid) const;
        // Entity GetParent(EntityID eid);
        // bool HasParent(EntityID eid) const;
        // bool IsParent(EntityID eid, EntityID parent) const;
        // void SetParent(EntityID eid, EntityID parent);
        // void RemoveParent(EntityID eid);

        // bool HasChild(EntityID eid, EntityID child) const;
        // const std::vector<Entity> GetChildren(EntityID eid) const;
        // std::vector<Entity> GetChildren(EntityID eid);

        operator EntityID() const;
    
    private:

        PCNode* pcnode = nullptr;
    };
}