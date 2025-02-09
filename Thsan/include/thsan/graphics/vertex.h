#pragma once
#include "thsan/core.h"
#include "glm/glm.hpp"

namespace Thsan {
    struct Vertex {
        glm::vec3 position{0.f};
        glm::vec3 normal{0.f};
        glm::vec2 texCoord{0.f};
        glm::vec3 tangent{0.f};
        glm::vec3 bitangent{0.f};
        glm::vec4 color{1.f};
        glm::vec3 externalPosition{ 0.f };
    };
}
