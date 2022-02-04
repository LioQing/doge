#pragma once

#include <doge/utils/aliases.hpp>
#include <doge/utils/lev.hpp>
#include <doge/utils/Vec2.hpp>
#include <doge/utils/Rect.hpp>
#include <doge/utils/Color.hpp>
#include <string>
#include <bitset>

namespace doge
{
    struct Engine;
    struct Entity;
    
    template <typename TComp>
    struct Component;

    struct Camera;

    namespace gui
    {
        struct GUI;
        
        struct Element
        {
            virtual ~Element() = 0;

            virtual void Initialize() {};
            virtual void Update(DeltaTime) {};
            virtual void FixedUpdate(DeltaTime) {};

            void ElementInitialize();

            GUI& GetGUI() const;
            Entity GetEntity() const;

            void SetLocalLayer(std::int32_t layer);
            void SetLayer(std::int32_t layer);
            std::int32_t GetLocalLayer() const;
            std::int32_t GetLayer() const;

            void SetCursorDetectable(bool is_cursor_detectable);
            bool IsCursorDetectable() const;

            const std::string& GetID() const;

            const std::string& GetCameraID() const;
            doge::Component<Camera>& GetCameraComponent() const;

            void SetSize(const Vec2f& size);
            const Vec2f& GetSize() const;

            void SetPosition(const Vec2f& position);
            const Vec2f& GetPosition() const;

            void SetOrigin(const Vec2f& origin);
            const Vec2f& GetOrigin() const;

            void SetColor(const Color& color);
            const Color& GetColor() const;

            Rectf GetRectangle() const;

        protected:

            virtual void OnLayerUpdated() {};
            virtual void OnCursorDetectableUpdated() {};
            virtual void OnSizeUpdated() {};
            virtual void OnPositionUpdated() {};
            virtual void OnOriginUpdated() {};
            virtual void OnColorUpdated() {};
            
        private:

            std::string id;
            std::string camera;
            bool is_cursor_detectable = true;
            Vec2f size = Vec2f::Zero;
            Vec2f origin = Vec2f::Zero;
            Color color = 0xFFFFFFFF;

            GUI* gui = nullptr;

            friend struct GUI;
        };
    }
}