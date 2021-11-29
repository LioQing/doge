#include <doge/core/Entity.hpp>

#include <stdexcept>

namespace doge
{
    Entity::Entity(lic::EntityID id) : lic::Entity(lic::Entity(id))
    {
    }

    // void Entity::SetParent(lic::EntityID id) const
    // {
        
    // }

    // void Entity::RemoveParent(lic::EntityID id) const
    // {
        
    // }

    // Entity Entity::GetParent() const 
    // {
        
    // }

    // Entity Entity::HasParent() const 
    // {
        
    // }

    Entity::operator lic::EntityID() const 
    {
        return lic::Entity::id;
    }
}