#include "pch.h"
#include "transform.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/quaternion_common.hpp>
#include <glm/gtx/matrix_decompose.hpp>

namespace tsm {

    TransformImpl::TransformImpl() {
        // Initialize transformation matrices
        translation_matrix = glm::mat4(1.0f);
        scale_matrix = glm::mat4(1.0f);
        rotation_matrix = glm::mat4(1.0f);
        transform_matrix = glm::mat4(1.0f);
        translate_to_origin_matrix = glm::mat4(1.0f);
        translate_back_matrix = glm::mat4(1.0f);
    }

    void TransformImpl::translate(const glm::vec3& translation) {
        // Update the translation matrix
        translation_matrix = glm::translate(glm::mat4(1.0f), translation);
    }

    void TransformImpl::scale(const glm::vec3& scale) {
        // Update the scale matrix
        scale_matrix = glm::scale(glm::mat4(1.0f), scale);
    }

    void TransformImpl::setTranslation(const glm::vec3& translation) {
        // Set the translation matrix directly
        translation_matrix = glm::translate(glm::mat4(1.0f), translation);
    }

    void TransformImpl::setScale(const glm::vec3& scale) {
        // Set the scale matrix directly
        scale_matrix = glm::scale(glm::mat4(1.0f), scale);
    }

    void TransformImpl::setRotation(const glm::vec3& rotate, float angle) {
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

    const glm::mat4 TransformImpl::getTransform() {

        glm::mat4 R = translate_back_matrix * rotation_matrix * translate_to_origin_matrix;
        transform_matrix = translation_matrix * R * scale_matrix;
        return transform_matrix;
    }

    std::shared_ptr<Transform> Transform::create()
    {
        return std::make_shared<TransformImpl>();
    }


}
