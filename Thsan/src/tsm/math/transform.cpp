#include "pch.h"
#include "tsm/math/transform.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/quaternion_common.hpp>
#include <glm/gtx/matrix_decompose.hpp>

namespace tsm {

    TransformImpl::TransformImpl() {
        // Initialize transformation matrices
        translation_matrix = glm::identity<glm::mat4>();
        scale_matrix = glm::identity<glm::mat4>();
        rotation_matrix = glm::identity<glm::mat4>();
        transform_matrix = glm::identity<glm::mat4>();
        translate_to_origin_matrix = glm::identity<glm::mat4>();
        translate_back_matrix = glm::identity<glm::mat4>();

        // self reminder, parce que mon brain oublie toujours
        // (1 0 0 0) - Identity quaternion, no rotation,
        // (0 1 0 0) - 180° turn around X axis,
        // (0 0 1 0) - 180° turn around Y axis,
        // (0 0 0 1) - 180° turn around Z axis,
        // source: https://stackoverflow.com/questions/41156626/quaternion-0-0-0-0-signifies-what

        rotation_quad = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    }

    void TransformImpl::translate(const glm::vec3& translation)
    {
        translation_matrix = glm::translate(glm::mat4(1.0f), translation);
    }

    void TransformImpl::scale(const glm::vec3& scale) 
    {
        scale_matrix = glm::scale(glm::mat4(1.0f), scale);
    }

    void TransformImpl::setTranslation(const glm::vec3& translation) 
    {
        translation_matrix = glm::translate(glm::mat4(1.0f), translation);
    }

    void TransformImpl::setScale(const glm::vec3& scale) 
    {
        scale_matrix = glm::scale(glm::mat4(1.0f), scale);
    }

    void TransformImpl::setRotation(const glm::vec3& rotate, float angle) 
    {
        glm::quat rotation_quad = glm::angleAxis(glm::radians(angle), rotate);
        rotation_matrix = glm::mat4_cast(rotation_quad);
    }

    void TransformImpl::setOrigin(const glm::vec3& origin)
    {
        this->translate_to_origin_matrix = glm::translate(glm::mat4(1.0f), -origin);
        this->translate_back_matrix = glm::translate(glm::mat4(1.0f), origin);
    }

    glm::vec3 TransformImpl::getTranslation()
    {
        return glm::vec3(transform_matrix[3]);
    }

    glm::vec3 TransformImpl::getScale()
    {
        glm::vec3 scale;
        scale.x = scale_matrix[0][0];
        scale.y = scale_matrix[1][1];
        scale.z = scale_matrix[2][2];
        return scale;
    }

    glm::mat4 TransformImpl::getRotation()
    {
        return rotation_matrix;
    }

    glm::vec3 TransformImpl::getOrigin()
    {
        return glm::vec3(transform_matrix[3]);
    }

    void TransformImpl::rotate(const glm::vec3& rotate, float angle) {
        rotation_matrix = glm::mat4_cast(rotation_quad * glm::angleAxis(glm::radians(angle), rotate));
    }

    const glm::mat4 TransformImpl::getTransform() 
    {
        glm::mat4 RS = translate_back_matrix * rotation_matrix * scale_matrix * translate_to_origin_matrix;
        transform_matrix = translation_matrix * RS;
        return transform_matrix;
    }

    std::shared_ptr<Transform> Transform::create()
    {
        return std::make_shared<TransformImpl>();
    }


}
