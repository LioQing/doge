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

        std::function<void(Engine&, DeltaTime)> early_update;
        std::function<void(Engine&, DeltaTime)> late_update;

        template <typename Obj,
            typename StartPtr = std::nullptr_t,
            typename UpdatesPtr = std::nullptr_t,
            typename FixedUpdatePtr = std::nullptr_t,
            typename FinishPtr = std::nullptr_t,
            typename EarlyUpdatePtr = std::nullptr_t,
            typename LateUpdatePtr = std::nullptr_t>
        static GameLoopFunctions Create(
            Obj& obj,
            StartPtr start = nullptr,
            UpdatesPtr update = nullptr,
            FixedUpdatePtr fixed_update = nullptr,
            FinishPtr finish = nullptr,
            EarlyUpdatePtr early_update = nullptr,
            LateUpdatePtr late_update = nullptr
        )
        {
            GameLoopFunctions glf;
            if constexpr (!std::is_same_v<StartPtr, std::nullptr_t>)        glf.start           = [=, &obj](Engine& engine){ std::invoke(start, obj, engine); };
            if constexpr (!std::is_same_v<UpdatesPtr, std::nullptr_t>)      glf.update          = [=, &obj](Engine& engine, DeltaTime dt){ std::invoke(update, obj, engine, dt); };
            if constexpr (!std::is_same_v<FixedUpdatePtr, std::nullptr_t>)  glf.fixed_update    = [=, &obj](Engine& engine, DeltaTime dt){ std::invoke(fixed_update, obj, engine, dt); };
            if constexpr (!std::is_same_v<FinishPtr, std::nullptr_t>)       glf.finish          = [=, &obj](Engine& engine){ std::invoke(finish, obj, engine); };
            if constexpr (!std::is_same_v<EarlyUpdatePtr, std::nullptr_t>)  glf.early_update    = [=, &obj](Engine& engine, DeltaTime dt){ std::invoke(early_update, obj, engine); };
            if constexpr (!std::is_same_v<LateUpdatePtr, std::nullptr_t>)   glf.late_update     = [=, &obj](Engine& engine, DeltaTime dt){ std::invoke(late_update, obj, engine); };
            return glf;
        }
    };
}