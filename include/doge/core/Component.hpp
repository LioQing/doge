#pragma once

#include "../utils/lic.hpp"
#include "PCNode.hpp"
#include "Entity.hpp"

namespace doge
{
    template <typename TComp>
    struct Component : public lic::Component<TComp>
    {
        Entity GetEntity() const
        {
            return Entity(this->entity_id, PCNode::root.GetDescendent(this->entity_id).get());
        }
    };
}