#include "pch.h"
#include "thsan/graphics/render_states.h"
#include "thsan/graphics/shader.h"
#include "thsan/graphics/texture.h"
#include "thsan/graphics/view.h"
#include "tsm/math/camera/abstract_camera.h"
#include "tsm/math/transform.h"
#include "thsan/ressource_manager/texture2D_manager.h"

namespace Thsan {

    std::weak_ptr<Shader> Thsan::RenderStatesImpl::getShader() const
    {
        return shader;
    }

    void Thsan::RenderStatesImpl::setShader(std::weak_ptr<Shader> shader)
    {
        this->shader = shader;
    }

    inline void RenderStatesImpl::setBlend(BlendMode blendMode)
    {
        this->blendMode = blendMode;
    }

    inline void RenderStatesImpl::setTransform(std::weak_ptr<tsm::Transform> transform)
    {
        this->transform = transform;
    }

    inline void RenderStatesImpl::setTexture(TextureType type, std::weak_ptr<Texture2D> texture)
    {
        textureMap[type] = texture;
    }

    inline void RenderStatesImpl::unsetTexture(TextureType type)
    {
        textureMap[type].reset();
    }

    inline void RenderStatesImpl::setView(std::weak_ptr<View> view)
    {
        this->view = view;
    }

    RendererType RenderStatesImpl::getRendererType()
    {
        return rendererType;
    }

    void RenderStatesImpl::bind() const
    {
        const std::shared_ptr<Shader> tmp_shader = getShader().lock();
        const std::shared_ptr<View> tmp_view = view.lock();
        const std::shared_ptr<tsm::Transform> tmp_transform = transform.lock();
        const std::shared_ptr<Texture2D> tmp_defaultTexture = textureMap.at(TextureType::Default).lock();
        const std::shared_ptr<Texture2D> tmp_depthTexture = textureMap.at(TextureType::DepthBuffer).lock();
        const std::shared_ptr<Texture2D> tmp_positionTexture = textureMap.at(TextureType::Position).lock();


        if(tmp_transform)
            tmp_shader->setMat4("model", tmp_transform->getTransform());
        else
            tmp_shader->setMat4("model", glm::identity<glm::mat4>());

        if (tmp_view) {
            tmp_shader->setMat4("view", tmp_view->getViewMatrix());
            tmp_shader->setFloat("maximumDepth", tmp_view->getMaximumDepth());
            tmp_shader->setMat4("projection", tmp_view->getProjectionMatrix());
        }
        else {
            tmp_shader->setMat4("view", glm::identity<glm::mat4>());
            const float depth = 100.f;
            tmp_shader->setFloat("maximumDepth", depth);
            tmp_shader->setMat4("projection", glm::identity<glm::mat4>());
            //TS_CORE_WARN("in RenderStatesImpl::bind(), no view was set, defaulting to identity matrix and depth of {}.", depth);
        }

        if (tmp_depthTexture) {
            tmp_shader->setTexture2D("textureDepth", tmp_depthTexture);
            tmp_shader->setBool("overwriteTextureDepth", true);

        }else
            tmp_shader->setBool("overwriteTextureDepth", false);

        if (tmp_positionTexture) {
            tmp_shader->setTexture2D("texturePosition", tmp_positionTexture);
            tmp_shader->setBool("overwriteTexturePosition", true);

        }
        else
            tmp_shader->setBool("overwriteTexturePosition", false);

        if (tmp_defaultTexture)
            tmp_shader->setTexture2D("textureDefault", tmp_defaultTexture);
        else
            tmp_shader->setTexture2D("textureDefault", RessourceManager::Texture2DManager::get(RessourceManager::Texture2DManager::default_texture_checkerboard));

        tmp_shader->bind();
    }

    void RenderStatesImpl::unBind() const
    {
        std::shared_ptr<Shader> tmp_shader = getShader().lock();
        tmp_shader->unBind();
    }

    RenderStatesImpl::RenderStatesImpl(RendererType type) :
        textureMap({
            { TextureType::DepthBuffer, std::weak_ptr<Texture2D>() },
            { TextureType::Position, std::weak_ptr<Texture2D>() },
            { TextureType::Default, std::weak_ptr<Texture2D>() }
            // Add other TextureType keys here
            }) 
    {
        rendererType = type;
    }

    std::shared_ptr<RenderStates> RenderStates::create(RendererType type)
    {
        return std::make_shared<RenderStatesImpl>(type);
    }

}
