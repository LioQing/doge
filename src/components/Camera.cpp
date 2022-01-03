#include <doge/components/Camera.hpp>

namespace doge
{
    Camera::Camera(const Vec2f& size, const Rectf& port) : size(size), port(port)
    {
    }

    Vec2f& Camera::GetSize()
    {
        is_modified = true;
        return size;
    }

    const Vec2f& Camera::GetSize() const
    {
        return size;
    }

    Rectf& Camera::GetPort()
    {
        is_modified = true;
        return port;
    }

    const Rectf& Camera::GetPort() const
    {
        return port;
    }

    Vec2f& Camera::_NoModify_GetSize()
    {
        return size;
    }

    Rectf& Camera::_NoModify_GetPort()
    {
        return port;
    }

    void Camera::_ResetModify()
    {
        is_modified = false;
    }

    bool Camera::_IsModified() const
    {
        return is_modified;
    }
}