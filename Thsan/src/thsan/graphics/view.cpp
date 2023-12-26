#include "pch.h"
#include "view.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace Thsan {

    std::shared_ptr<View> View::create(float x, float y, float width, float height, float zoom, float rotation) {
        return std::make_shared<ViewImpl>(x, y, width, height, zoom, rotation);
    }

    ViewImpl::ViewImpl(float x, float y, float width, float height, float zoom, float rotation)
        : position(x, y), size(width, height), zoomFactor(zoom), rotationAngle(rotation) {}

    void ViewImpl::setPosition(float x, float y) {
        position.x = x;
        position.y = y;
    }

    void ViewImpl::setSize(float width, float height) {
        size.x = width;
        size.y = height;
    }

    void ViewImpl::setZoom(float zoom) {
        zoomFactor = zoom;
    }

    void ViewImpl::setRotation(float angle) {
        rotationAngle = angle;
    }

    glm::mat4 ViewImpl::getViewMatrix() const {
        glm::mat4 viewMatrix(1.0f);

        viewMatrix = glm::translate(viewMatrix, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
        viewMatrix = glm::scale(viewMatrix, glm::vec3(zoomFactor, zoomFactor, 1.0f));

        viewMatrix = glm::rotate(viewMatrix, glm::radians(rotationAngle), glm::vec3(0.0f, 0.0f, 1.0f));
        viewMatrix = glm::translate(viewMatrix, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

        viewMatrix = glm::translate(viewMatrix, glm::vec3(-position, 0.0));
        return viewMatrix;
    }


    glm::mat4 ViewImpl::getProjectionMatrix() const {
        // Adjust the projection matrix
        return glm::ortho(0.f, size.x, size.y, 0.f, -1000.f, 1000.f);
    }


    glm::mat4 ViewImpl::getViewProjectionMatrix() const {
        glm::mat4 projectionMatrix = getProjectionMatrix();
        glm::mat4 viewMatrix = getViewMatrix();
        return projectionMatrix * viewMatrix;
    }


    glm::vec2 ViewImpl::getPosition()
    {
        return position;
    }

    glm::vec2 ViewImpl::getSize()
    {
        return size;
    }

    float ViewImpl::getZoom()
    {
        return zoomFactor;
    }

    float ViewImpl::getRotation()
    {
        return rotationAngle;
    }
}