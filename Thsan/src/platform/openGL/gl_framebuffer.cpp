#include "pch.h"
#include "thsan/graphics/texture.h"
#include "thsan/log.h"
#include "gl_framebuffer.h"
#include "GL/glew.h"
#include "gl_helper.h"
#include "gl_texture2D.h"
#include <thsan/ressource_manager/texture2D_manager.h>

namespace Thsan {

    GLFramebuffer::GLFramebuffer(uint32_t width, uint32_t height) :
        Framebuffer(width, height)
    {
        GL_CHECK(glGenFramebuffers(1, &fbo));
    }

    GLFramebuffer::~GLFramebuffer()
    {
        if (fbo != 0) {
            GL_CHECK(glDeleteFramebuffers(1, &fbo));
            fbo = 0;
        }

        if (depthStencilRenderbuffer != 0) {
            GL_CHECK(glDeleteRenderbuffers(1, &depthStencilRenderbuffer));
            depthStencilRenderbuffer = 0;
        }

        for (auto& colorAttachment : colorAttachments)
            if (colorAttachment)
                colorAttachment.reset();

        colorAttachments.clear();
    }

    void GLFramebuffer::bind()
    {
        GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, fbo));
    }

    void GLFramebuffer::Unbind()
    {
        GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    }

    void GLFramebuffer::attachColorTarget(int attachmentIndex, TextureFormat format)
    {
        GLint previousFramebuffer;
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &previousFramebuffer);

        GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, fbo));

        if (attachmentIndex >= colorAttachments.size()) {
            TS_CORE_ERROR("in Framebuffer::attachColorTarget, attachmentIndex is {} which is bigger than the numColorAttachments that is {}, so aborted buddy\n", attachmentIndex, colorAttachments.size());
            return;
        }
        auto it = FormatMapping.find(format);
        if (it != FormatMapping.end()) {
            auto& [internalFormat, glFormat, glType] = it->second;

            std::stringstream ss;
            ss << "fbo" << fbo;
            ss << "_attachmentIndex" << attachmentIndex;
            ss << "_internalFormat" << internalFormat;
            ss << "_format" << glFormat;
            ss << "_type" << glType;
            std::string name = ss.str();

            auto texture = RessourceManager::Texture2DManager::create(name, { glm::uvec2(width, height), format, nullptr});

            GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachmentIndex, GL_TEXTURE_2D, texture->getID(), 0));

            colorAttachments[attachmentIndex] = texture;
        }

        GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, previousFramebuffer));
    }

    void GLFramebuffer::attachDepthStencilTarget()
    {
        GLint previousFramebuffer;
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &previousFramebuffer);

        GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, fbo));

        std::vector<GLenum> attachments;
        for (int i = 0; i < colorAttachments.size(); i++)
            attachments.push_back(GL_COLOR_ATTACHMENT0 + i);

        GL_CHECK(glDrawBuffers(static_cast<GLsizei>(attachments.size()), attachments.data()));

        GL_CHECK(glGenRenderbuffers(1, &depthStencilRenderbuffer));
        GL_CHECK(glBindRenderbuffer(GL_RENDERBUFFER, depthStencilRenderbuffer));

        GL_CHECK(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height));
        GL_CHECK(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthStencilRenderbuffer));
        GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, previousFramebuffer));
    }


    void GLFramebuffer::SetNumColorAttachments(int numAttachments)
    {
        if (numAttachments <= 0)
            return;

        while (colorAttachments.size() > numAttachments)
            colorAttachments.pop_back();
        while (colorAttachments.size() < numAttachments)
            colorAttachments.push_back(nullptr);
    }

    std::shared_ptr<Texture2D> GLFramebuffer::getTexture(int attachmentIndex) const
    {
        if (attachmentIndex < 0 || attachmentIndex >= colorAttachments.size())
            return nullptr;

        return colorAttachments[attachmentIndex];
    }

    std::shared_ptr<Texture2D> GLFramebuffer::getDepthTexture() const
    {
        return depthAttachment;
    }
}
