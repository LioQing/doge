#pragma once

namespace doge
{
    enum StateMachineAutoUpdate
    {
        None = 0,
        Update = 1 << 0,
        FixedUpdate = 1 << 1,
        All = Update | FixedUpdate,
    };
}