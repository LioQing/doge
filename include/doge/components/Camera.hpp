#pragma once

#include <doge/utils/Vec2.hpp>
#include <doge/utils/Rect.hpp>

namespace doge
{
    class Camera
    {
        // size of camera in scene
        Vec2f size = Vec2f::Zero(); // (0, 0) represent auto resize corresponding to port rect
        
        // size of camera in window (in ratio to the window)
        Rectf port = Rectf(0, 0, 1, 1);

        bool is_modified = true;

    public:

        Camera(const Camera&) = default;
        Camera(const Vec2f& size = Vec2f::Zero(), const Rectf& port = Rectf(0, 0, 1, 1));

        Vec2f& GetSize();
        const Vec2f& GetSize() const;

        Rectf& GetPort();
        const Rectf& GetPort() const;

        Vec2f& _NoModify_GetSize();
        Rectf& _NoModify_GetPort();
        void _ResetModify();
        bool _IsModified() const;
    };
}