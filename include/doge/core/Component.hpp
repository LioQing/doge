#pragma once

#include <doge/utils/lic.hpp>
#include <doge/core/PCNode.hpp>
#include <doge/core/Entity.hpp>

namespace doge
{
    template <typename TComp>
    struct Component : public lic::Component<TComp>
    {
        Entity GetEntity() const
        {
            return Entity(this->entity_id, PCNode::root.GetDescendent(this->entity_id).get());
        }

        TComp& Get()
        {
            return static_cast<TComp&>(*this);
        }
    };
}