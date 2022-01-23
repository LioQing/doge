#pragma once

#include <doge/utils/aliases.hpp>
#include <doge/utils/lev.hpp>
#include <doge/utils/Vec2.hpp>
#include <doge/utils/Rect.hpp>
#include <string>

namespace doge
{
    struct Engine;
    
    template <typename TComp>
    struct Component;

    struct Camera;

    struct GUIElement
    {
        enum ScaleVec
        {
            None = 0,
            Size = 1 << 0,
            Position = 1 << 1,
            Origin = 1 << 2,

            All = Size | Position | Origin,
        };

        virtual ~GUIElement() = 0;

        virtual void Initialize(Engine&) {};
        virtual void Update(Engine&, DeltaTime) {};
        virtual void FixedUpdate(Engine&, DeltaTime) {};

        void SetID(const std::string& id);
        const std::string& GetID() const;

        void SetTextureID(const std::string& texture_id);
        const std::string& GetTextureID() const;

        void SetCamera(const std::string& owner_camera);
        const std::string& GetCamera() const;
        doge::Component<Camera>& GetCameraComponent() const;

        void SetSize(const Vec2f& size);
        const Vec2f& GetSize() const;

        void SetPosition(const Vec2f& position);
        const Vec2f& GetPosition() const;

        // size & position scale with screen size, origin scale with size
        void SetIsScaleVec(ScaleVec is_scale_vec);
        ScaleVec IsScaleVec() const;
        bool IsScaleVec(ScaleVec scale_vec) const;

        Rectf GetRectangle() const;

    private:

        std::string id;
        std::string texture_id;
        std::string camera;
        Vec2f size;
        Vec2f position;
        Vec2f origin;
        ScaleVec is_scale_vec = ScaleVec::None;
    };
}