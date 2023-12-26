#pragma once
#include "thsan/core.h"
#include "thsan/system/vector.h"
namespace Thsan {

    struct THSAN_API Vertex {
        vec3f position;
        vec3f normal;
        vec2f texCoord;
        vec3f tangent;
        vec3f bitangent;
        vec4f color;
    };
}

/*
namespace Thsan {
    class Mesh;

    class THSAN_API Vertex {
    public:
        enum class TypeVec3 {
            position,
            normal,
            tangent,
            bitangent
        };

        enum class TypeVec2 {
            texCoord
        };

        virtual glm::vec3 getAttribute(TypeVec3 type) = 0;
        virtual glm::vec2 getAttribute(TypeVec2 type) = 0;

        virtual int getAttributeOffset(TypeVec3 type) = 0;
        virtual int getAttributeOffset(TypeVec2 type) = 0;
        virtual int getVertexDataSize() = 0;
        virtual const void* getData() = 0;
    };

    class VertexImpl: public Vertex {
    public:
        VertexImpl();
        ~VertexImpl();

        glm::vec3 getAttribute(Vertex::TypeVec3 type) override;
        glm::vec2 getAttribute(Vertex::TypeVec2 type) override;

        int getAttributeOffset(TypeVec3 type) override;
        int getAttributeOffset(TypeVec2 type) override;
        int getVertexDataSize() override;
        const void* getData() override;

    private:
        struct VertexData {
            glm::vec3 position;
            glm::vec3 normal;
            glm::vec2 texCoord;
            glm::vec3 tangent;
            glm::vec3 bitangent;
        };

        VertexData vertex_data;
    };
}
*/