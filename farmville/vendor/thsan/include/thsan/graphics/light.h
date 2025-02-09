#pragma once
#include <glm/glm.hpp>
#include <tsm/math/color.h>

struct LightProperties {
    bool isEnable;
    bool isDirection;
    bool isPoint;
    bool isSpot;
    bool isCursor;

   glm::vec3 position;
   glm::vec3 direction;

    float attenuationConstant;
    float attenuationLinear;
    float attenuationQuadratic;

    tsm::Color ambient;
    tsm::Color diffuse;
    tsm::Color specular;
};