#pragma once
#include "thsan/core.h"
#include "thsan/log.h"

#include <string>
#include <glm/glm.hpp>
#include <tsm/tsm.h>

namespace Thsan {
    class Texture2D;
    class Mesh;

    class THSAN_API ComputeShader {
    public:
        ComputeShader() = default;
        ~ComputeShader() = default;

        virtual void use() = 0;
        virtual void dispatch(unsigned int x, unsigned int y, unsigned int z) = 0;
        virtual void wait() = 0;

        virtual void setBool(const std::string& name, bool value) = 0;
        virtual void setInt(const std::string& name, int value) = 0;
        virtual void setFloat(const std::string& name, float value) = 0;

        virtual void setMat3(const std::string& name, glm::mat3 value) = 0;
        virtual void setMat4(const std::string& name, glm::mat4 value) = 0;

        virtual void setVec2(const std::string& name, glm::vec2 value) = 0;
        virtual void setVec3(const std::string& name, glm::vec3 value) = 0;
        virtual void setVec4(const std::string& name, glm::vec4 value) = 0;

        virtual void setCamera(std::weak_ptr<tsm::AbstractCamera> camera);
        virtual void setTexture2D(const std::string& name, std::weak_ptr<Texture2D> tex2D) = 0;
        virtual void setImage2D(std::weak_ptr<Texture2D> tex2D, int bindingPoint) = 0;

        virtual void setRWImage2D(std::weak_ptr<Texture2D> tex2D, int bindingPoint) = 0;
        virtual void setRWMesh(std::weak_ptr<Mesh> mesh, int bindingPoint) = 0;

        std::shared_ptr<ComputeShader> static create(const std::string& cs_path);

    protected:
        std::string readFile(std::string path);
    };
}

