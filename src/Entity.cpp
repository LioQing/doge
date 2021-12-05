#include <doge/core/Entity.hpp>

#include <stdexcept>
#include <doge/core/Engine.hpp>

namespace doge
{
    Entity::Entity(EntityID id) : lic::Entity(lic::Entity(id))
    {
    }

    // void Entity::SetParent(EntityID id) const
    // {
        
    // }

    // void Entity::RemoveParent(EntityID id) const
    // {
        
    // }

    // Entity Entity::GetParent() const 
    // {
        
    // }

    // Entity Entity::HasParent() const 
    // {
        
    // }

    Entity::operator EntityID() const 
    {
        return lic::Entity::id;
    }
}