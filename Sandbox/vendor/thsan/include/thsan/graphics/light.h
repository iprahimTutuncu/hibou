#pragma once
#include <glm/glm.hpp>
#include <tsm/math/color.h>
#include <thsan/core.h>

namespace Thsan
{
    struct THSAN_API LightProperties
    {
        bool isValid{ false };
        bool isEnabled{ true };
        bool isDirectional{ false };
        bool isPoint{ true };
        bool isSpot{ false };

        glm::vec3 position{ 0.0f, 5.0f, 0.0f };
        glm::vec3 direction{ 0.0f, -1.0f, 0.0f };

        float intensity{ 10.0f };
        float radius{ 1.0f };
        float falloff{ 1.0f };

        float cutoffAngle{ glm::radians(30.0f) };
        float outerCutoffAngle{ glm::radians(45.0f) };

        tsm::Color radiance{ 255, 255, 255 };
        tsm::Color diffuse{ 255, 255, 255 };
        tsm::Color specular{ 255, 255, 255 };
    };

}
