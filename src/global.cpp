#include <doge/utils/global.hpp>

namespace doge
{
    Vec2f global::GetPosition(lic::EntityID eid)
    {
        return lic::GetEntity(eid).GetIfHasComponentElseDefault<Position>().position;
    }

    Vec2f global::GetScale(lic::EntityID eid)
    {
        return lic::GetEntity(eid).GetIfHasComponentElseDefault<Scale>().scale;
    }

    float global::GetRotation(lic::EntityID eid)
    {
        return lic::GetEntity(eid).GetIfHasComponentElseDefault<Rotation>().rotation;
    }
}