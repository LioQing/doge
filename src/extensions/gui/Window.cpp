#include <doge/extensions/gui/Window.hpp>

#include <doge/extensions/gui/GUI.hpp>
#include <algorithm>

namespace doge::gui
{
    const Vec2f Window::DefaultSize = Vec2f(600, 480);

    Window::~Window()
    {

    }

    void Window::Initialize()
    {
        camera_entity = GetGUI().GetEngine().AddCamera();

        // layer
        auto& parent_cam_layers = GetGUI().GetCameraLayers(GetCameraID());
        std::set<std::int32_t> layers;
        for (auto& layer : parent_cam_layers)
            layers.emplace(layer + *std::max_element(parent_cam_layers.begin(), parent_cam_layers.end()));
        
        camera_entity.AddComponent<Layer>(layers);

        // sprite

        SetSize(DefaultSize);
    }

    void Window::OnSizeUpdated()
    {
        camera_entity.GetComponent<Camera>().size = GetSize();
        camera_entity.GetComponent<Camera>().port.SetSize(GetSize() / GetCameraComponent().size);
    }
}