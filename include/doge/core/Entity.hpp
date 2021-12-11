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

        Entity GetParent() const;
        bool HasParent() const;
        bool IsParent(EntityID parent) const;
        void SetParent(EntityID parent) const;
        void RemoveParent() const;

        bool HasChild(EntityID child) const;
        std::vector<Entity> GetChildren() const;

        operator EntityID() const;
    
    private:

        PCNode* pcnode = nullptr;
    };
}