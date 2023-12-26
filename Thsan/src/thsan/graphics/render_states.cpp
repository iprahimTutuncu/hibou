#include "pch.h"
#include "render_states.h"
#include "shader.h"
#include "texture.h"
#include "view.h"
#include "tsm/math/camera/abstract_camera.h"
#include "tsm/math/transform.h"

namespace Thsan {

    std::weak_ptr<Shader> Thsan::RenderStates2DImpl::getShader() const
    {
        return shader;
    }

    void Thsan::RenderStates2DImpl::setShader(std::weak_ptr<Shader> shader)
    {
        this->shader = shader;
    }

    inline void RenderStates2DImpl::setTransform(std::weak_ptr<tsm::Transform> transform)
    {
        this->transform = transform;
    }

    inline void RenderStates2DImpl::setView(std::weak_ptr<View> view)
    {
        this->view = view;
    }

    void RenderStates2DImpl::bind() const
    {
        const std::shared_ptr<Shader> tmp_shader = getShader().lock();
        const std::shared_ptr<View> tmp_view = view.lock();
        const std::shared_ptr<tsm::Transform> tmp_transform = transform.lock();

        tmp_shader->bind();

        if(tmp_transform)
            tmp_shader->setMat4("model", tmp_transform->getTransform());
        else
            tmp_shader->setMat4("model", glm::identity<glm::mat4>());

        if (tmp_view)
            tmp_shader->setMat4("view", tmp_view->getViewMatrix());
        else {
            tmp_shader->setMat4("view", glm::identity<glm::mat4>());
            TS_CORE_WARN("in RenderStates2DImpl::bind(), no view was set, defaulting to identity matrix.");
        }
        tmp_shader->setMat4("projection", tmp_view->getProjectionMatrix());
    }

    void RenderStates2DImpl::unBind() const
    {
        std::shared_ptr<Shader> tmp_shader = getShader().lock();
        tmp_shader->unBind();
    }

    inline std::weak_ptr<Shader> RenderStates3DImpl::getShader() const
    {
        return shader;
    }

    inline std::weak_ptr<tsm::AbstractCamera> RenderStates3DImpl::getCamera() const
    {
        return camera;
    }


    inline void RenderStates3DImpl::setShader(std::weak_ptr<Shader> shader)
    {
        this->shader = shader;
    }

    inline void RenderStates3DImpl::setCamera(std::weak_ptr<tsm::AbstractCamera> camera)
    {
        this->camera = camera;
    }
    void RenderStates3DImpl::bind() const
    {
        const std::shared_ptr<Shader> tmp_shader = getShader().lock();

        tmp_shader->bind();
        tmp_shader->setCamera(camera);
        tmp_shader->setMat4("transform", transform);
    }
    void RenderStates3DImpl::unBind() const
    {
        std::shared_ptr<Shader> tmp_shader = getShader().lock();
        tmp_shader->unBind();
    }


    std::shared_ptr<RenderStates3D> create_renderstates3D()
    {
        return std::shared_ptr<RenderStates3DImpl>();
    }

    std::shared_ptr<RenderStates2D> create_renderstates2D()
    {
        return std::make_shared<RenderStates2DImpl>();
    }

}
