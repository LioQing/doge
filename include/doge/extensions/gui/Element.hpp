#pragma once

#include <doge/utils/aliases.hpp>
#include <doge/utils/lev.hpp>
#include <doge/utils/Vec2.hpp>
#include <doge/utils/Rect.hpp>
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

            virtual std::int32_t GetLayer() const;

            GUI& GetGUI() const;
            Entity GetEntity() const;

            const std::string& GetID() const;

            const std::string& GetCameraID() const;
            doge::Component<Camera>& GetCameraComponent() const;

            void SetSize(const Vec2f& size);
            const Vec2f& GetSize() const;

            void SetPosition(const Vec2f& position);
            const Vec2f& GetPosition() const;

            void SetOrigin(const Vec2f& origin);
            const Vec2f& GetOrigin() const;

            Rectf GetRectangle() const;

        protected:

            virtual void OnSizeUpdated() {};
            virtual void OnPositionUpdated() {};
            virtual void OnOriginUpdated() {};
            
        private:

            std::string id;
            std::string camera;
            Vec2f size;
            Vec2f position;
            Vec2f origin;

            GUI* gui = nullptr;

            friend struct GUI;
        };
    }
}