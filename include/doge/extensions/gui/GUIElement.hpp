#pragma once

#include <doge/utils/aliases.hpp>
#include <string>
#include <doge/utils/lev.hpp>
#include <doge/utils/Vec2.hpp>

namespace doge
{
    struct Engine;

    struct GUIElement
    {
        virtual ~GUIElement() = 0;

        void SetID(const std::string& id);
        const std::string& GetID() const;

        void SetTextureID(const std::string& texture_id);
        const std::string& GetTextureID() const;

        void SetLayer(int layer);
        int GetLayer() const;

        void SetSize(const Vec2f& size);
        const Vec2f& GetSize() const;

        void SetPosition(const Vec2f& position);
        const Vec2f& GetPosition() const;

        virtual void Start(Engine&) {};
        virtual void Update(Engine&, DeltaTime) {};
        virtual void Finish(Engine&) {};

    private:

        std::string id;
        std::string texture_id;
        int layer;
        Vec2f size;
        Vec2f position;
    };
}