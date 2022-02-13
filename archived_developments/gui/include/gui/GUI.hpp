#pragma once

#include <doge/utils/aliases.hpp>
#include <doge/extensions/nine_slice.hpp>
#include <doge/extensions/gui/ElementComponent.hpp>
#include <doge/extensions/gui/Element.hpp>
#include <doge/extensions/gui/CursorDetectableElement.hpp>
#include <doge/core/Engine.hpp>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <memory>
#include <doge/utils/aliases.hpp>

namespace doge
{
    struct Camera;

    namespace gui
    {
        struct Text;
        struct Window;
        struct Camera;

        struct GUI
        {
            GUI(Engine& engine);
            ~GUI();

            Camera& AddCamera(const std::string& id);

            void RemoveCamera(const std::string& id);

            Camera& GetCamera(const std::string& id) const;
            doge::Component<doge::Camera>& GetCameraComponent(const std::string& id) const;
            Entity GetCameraEntity(const std::string& id) const;

            bool HasCamera(const std::string& id) const;

            template <std::derived_from<Element> E>
            E& AddElement(const std::string& id, const std::string& cam_id)
            {
                auto cam_itr = cameras.find(cam_id);
                if (cam_itr == cameras.end())
                    throw std::invalid_argument("Camera ID of gui::element is not found in GUI::cameras");

                auto entity = engine.AddEntity();

                auto& comp = entity.AddComponent(ElementComponent::Create(E()));
                comp.element->id = id;
                comp.element->camera = cam_id;
                comp.element->gui = this;

                if constexpr (std::is_base_of_v<CursorDetectableElement, E>)
                {
                    auto [itr, success] = cursor_detectable_elements.insert(std::static_pointer_cast<CursorDetectableElement>(comp.element));
                    
                    (*itr)->get_camera_to_be_focused = [ptr = *itr]() -> Camera& { return ptr->GetCamera(); };

                    comp.OnRemoval([&, val_id = id, itr]()
                    {
                        elements.erase(val_id);
                        if (GetElementBelowCursor() == *itr)
                            element_below_cursor = nullptr;
                        cursor_detectable_elements.erase(itr);
                    });
                }
                else
                {
                    comp.OnRemoval([&, val_id = id]()
                    {
                        elements.erase(val_id);
                    });
                }

                elements.emplace(comp.element->GetID(), entity);
                comp.element->ElementInitialize();

                return static_cast<E&>(*comp.element);
            }

            void RemoveElement(const std::string& id);
            void RemoveElements(const std::string& camera_id);

            Element& GetElement(const std::string& id) const;
            doge::Component<ElementComponent>& GetElementComponent(const std::string& id) const;
            Entity GetElementEntity(const std::string& id) const;

            bool HasElement(const std::string& id) const;

            Engine& GetEngine() const;
            doge::nine_slice::NineSlice& GetNineSlice();
            const doge::nine_slice::NineSlice& GetNineSlice() const;

            std::shared_ptr<CursorDetectableElement> GetElementBelowCursor() const;
            void SetElementBelowCursorLocked(bool locked);
            bool IsElementBelowCursorLocked() const;

            Camera* GetCameraFocused() const;

        private:

            Engine& engine;
            doge::nine_slice::NineSlice nine_slice;

            std::shared_ptr<CursorDetectableElement> element_below_cursor = nullptr;
            std::bitset<2> cursor_event_called;
            bool element_below_cursor_locked = false;
            Camera* camera_focused = nullptr;

            std::unordered_map<std::string, Entity> cameras;
            std::unordered_map<std::string, Entity> elements;
            std::unordered_set<std::shared_ptr<CursorDetectableElement>> cursor_detectable_elements;

            void Start(Engine& engine);
            void Update(Engine& engine, DeltaTime dt);
            void FixedUpdate(Engine& engine, DeltaTime dt);
            void Finish(Engine& engine);
        };
    }
}