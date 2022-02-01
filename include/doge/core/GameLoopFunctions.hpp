#pragma once

#include <functional>
#include <concepts>
#include <doge/utils/aliases.hpp>

namespace doge
{
    struct Engine;

    struct GameLoopFunctions
    {
        std::function<void(Engine&)> start;
        std::function<void(Engine&, DeltaTime)> update;
        std::function<void(Engine&, DeltaTime)> fixed_update;
        std::function<void(Engine&)> finish;

        template <typename Obj, typename StartPtr, typename UpdatesPtr, typename FixedUpdatePtr, typename FinishPtr>
        static GameLoopFunctions Create(
            Obj& obj,
            StartPtr start = nullptr,
            UpdatesPtr update = nullptr,
            FixedUpdatePtr fixed_update = nullptr,
            FinishPtr finish = nullptr
        )
        {
            GameLoopFunctions glf;
            if constexpr (!std::is_same_v<StartPtr, std::nullptr_t>)        glf.start           = [=, &obj](Engine& engine){ std::invoke(start, obj, engine); };
            if constexpr (!std::is_same_v<UpdatesPtr, std::nullptr_t>)      glf.update          = [=, &obj](Engine& engine, DeltaTime dt){ std::invoke(update, obj, engine, dt); };
            if constexpr (!std::is_same_v<FixedUpdatePtr, std::nullptr_t>)  glf.fixed_update    = [=, &obj](Engine& engine, DeltaTime dt){ std::invoke(fixed_update, obj, engine, dt); };
            if constexpr (!std::is_same_v<FinishPtr, std::nullptr_t>)       glf.finish          = [=, &obj](Engine& engine){ std::invoke(finish, obj, engine); };
            return glf;
        }
    };
}