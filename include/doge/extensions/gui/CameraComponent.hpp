#pragma once

#include <memory>
#include <doge/extensions/gui/Camera.hpp>

namespace doge::gui
{
    struct CameraComponent
    {
        std::shared_ptr<Camera> camera;
    };
}