#pragma once
#include "thsan/core.h"

namespace Thsan {
    struct THSAN_API vec2f {
        float x;
        float y;
    };

    struct THSAN_API vec3f {
        float x;
        float y;
        float z;
    };

    struct THSAN_API vec4f {
        float x;
        float y;
        float z;
        float w;
    };

    struct THSAN_API vec2u {
        uint32_t x;
        uint32_t y;
    };
}