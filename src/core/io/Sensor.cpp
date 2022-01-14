#include <doge/core/io/Sensor.hpp>

#include <doge/utils.hpp>
#include <doge/core/io.hpp>

namespace doge::io
{
    bool Sensor::IsAvailable(Type sensor)
    {
        return sf::Sensor::isAvailable(cast::ToSfSensorType(sensor));
    }

    void Sensor::SetEnabled(Type sensor, bool enabled)
    {
        sf::Sensor::setEnabled(cast::ToSfSensorType(sensor), enabled);
    }

    Vec3f Sensor::GetValue(Type sensor)
    {
        return cast::FromSfVec3(sf::Sensor::getValue(cast::ToSfSensorType(sensor)));
    }
}