#pragma once

#include <doge/utils/aliases.hpp>
#include <string>
#include <doge/utils/lev.hpp>

namespace doge
{
    struct Engine;

    struct GUIElement
    {
        virtual ~GUIElement() = 0;

        void SetID(const std::string& id);
        const std::string& GetID() const;

        virtual void Start(Engine&) {};
        virtual void Update(Engine&, DeltaTime) {};
        virtual void Finish(Engine&) {};

    private:

        std::string id;
    };
}