#pragma once
#include <glm/glm.hpp>

struct Material
{
    glm::vec3 albedo = glm::vec3(1.0f);   // Diffuse color
    float roughness = 0.5f;               // Roughness factor (0 = smooth, 1 = rough)
    glm::vec3 emission = glm::vec3(0.0f); // Emission color for light sources

    virtual ~Material() = default;
    virtual bool isEmitter() const { return false; }
};

struct EmitterMaterial : public Material
{
    EmitterMaterial(const glm::vec3& color, float intensity)
    {
        emission = color * intensity;
    }

    bool isEmitter() const override { return true; }
};

struct DiffuseMaterial : public Material
{
    DiffuseMaterial(const glm::vec3& color)
    {
        albedo = color;
    }
};
