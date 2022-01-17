#include <doge/extensions/physics/PhysicsCollider.hpp>

#include <doge/extensions/physics/physics.hpp>

namespace doge
{
    void PhysicsCollider::SetSensor(bool sensor)
    {
        b2_fixture->SetSensor(sensor);
    }

    bool PhysicsCollider::IsSensor() const
    {
        return b2_fixture->IsSensor();
    }

    bool PhysicsCollider::TestPoint(const Vec2f& point)
    {
        return b2_fixture->TestPoint(physics::ToB2Vec2(point));
    }
}