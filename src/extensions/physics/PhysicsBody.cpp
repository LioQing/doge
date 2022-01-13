#include <doge/extensions/physics/PhysicsBody.hpp>

#include <doge/utils.hpp>

namespace doge
{
    void PhysicsBody::SetTransform(const Vec2f& position, float rotation)
    {
        b2_body->SetTransform(cast::ToB2Vec2(position), rotation);
    }

    Vec2f PhysicsBody::GetPosition() const
    {
        return cast::FromB2Vec2(b2_body->GetPosition());
    }

    float PhysicsBody::GetRotation() const
    {
        return b2_body->GetAngle();
    }

    Vec2f PhysicsBody::GetCenterOfMass() const
    {
        return cast::FromB2Vec2(b2_body->GetWorldCenter());
    }

    Vec2f PhysicsBody::GetLocalCenterOfMass() const
    {
        return cast::FromB2Vec2(b2_body->GetLocalCenter());
    }

    void PhysicsBody::SetVelocity(const Vec2f& velocity)
    {
        b2_body->SetLinearVelocity(cast::ToB2Vec2(velocity));
    }

    Vec2f PhysicsBody::GetVelocity() const
    {
        return cast::FromB2Vec2(b2_body->GetLinearVelocity());
    }

    void PhysicsBody::SetAngularVelocity(float angular_velocity)
    {
        b2_body->SetAngularVelocity(angular_velocity);
    }

    float PhysicsBody::GetAngularVelocity() const
    {
        return b2_body->GetAngularVelocity();
    }

    float PhysicsBody::GetMass() const
    {
        return b2_body->GetMass();
    }

    float PhysicsBody::GetMomentOfInertia() const
    {
        return b2_body->GetInertia();
    }

    void PhysicsBody::SetDamping(float damping)
    {
        b2_body->SetLinearDamping(damping);
    }

    float PhysicsBody::GetDamping() const
    {
        return b2_body->GetLinearDamping();
    }

    void PhysicsBody::SetAngularDamping(float damping)
    {
        b2_body->SetAngularDamping(damping);
    }

    float PhysicsBody::GetAngularDamping() const
    {
        return b2_body->GetAngularDamping();
    }

    void PhysicsBody::ApplyForce(const Vec2f& force)
    {
        b2_body->ApplyForceToCenter(cast::ToB2Vec2(force), true);
    }

    void PhysicsBody::ApplyForce(const Vec2f& force, const Vec2f& position)
    {
        b2_body->ApplyForce(cast::ToB2Vec2(force), cast::ToB2Vec2(position), true);
    }

    void PhysicsBody::ApplyAcceleration(const Vec2f& acceleration)
    {
        ApplyForce(acceleration * GetMass());
    }

    void PhysicsBody::ApplyAcceleration(const Vec2f& acceleration, const Vec2f& position)
    {
        ApplyForce(acceleration * GetMass(), position);
    }

    void PhysicsBody::ApplyImpulse(const Vec2f& impulse)
    {
        b2_body->ApplyLinearImpulseToCenter(cast::ToB2Vec2(impulse), true);
    }

    void PhysicsBody::ApplyImpulse(const Vec2f& impulse, const Vec2f& position)
    {
        b2_body->ApplyLinearImpulse(cast::ToB2Vec2(impulse), cast::ToB2Vec2(position), true);
    }

    void PhysicsBody::ApplyTorque(float torque)
    {
        b2_body->ApplyTorque(torque, true);
    }

    void PhysicsBody::ApplyAngularAcceleration(float angular_acceleration)
    {
        b2_body->ApplyTorque(angular_acceleration * GetMomentOfInertia(), true);
    }

    void PhysicsBody::ApplyAngularImpulse(float impulse)
    {
        b2_body->ApplyAngularImpulse(impulse, true);
    }
}