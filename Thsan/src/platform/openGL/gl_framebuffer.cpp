#include "pch.h"
#include "thsan/graphics/texture.h"
#include "thsan/log.h"
#include "platform/openGL/gl_framebuffer.h"
#include "GL/glew.h"
#include "platform/openGL/gl_helper.h"
#include "platform/openGL/gl_texture2D.h"
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

        GL_CHECK(glEnable(GL_DEPTH_TEST));
        GL_CHECK(glDepthFunc(GL_LEQUAL));

        GL_CHECK(glEnable(GL_BLEND));
        GL_CHECK(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        GL_CHECK(glDepthMask(GL_TRUE));
    }

    void GLFramebuffer::Unbind()
    {
        GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    }

    void GLFramebuffer::attachColorTarget(int attachmentIndex, TextureFormat format, glm::vec4 clearColor)
    {
        GLint previousFramebuffer;
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &previousFramebuffer);

        GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, fbo));

        if (attachmentIndex >= colorAttachments.size()) {
            TS_CORE_ERROR("in Framebuffer::attachColorTarget, attachmentIndex is {} which is bigger than the numColorAttachments that is {}, so aborted buddy\n", attachmentIndex, colorAttachments.size());
            return;
        }
        auto it = GLTexture2D::FormatMapping.find(format);
        if (it != GLTexture2D::FormatMapping.end()) {
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
            clearColors[attachmentIndex] = clearColor;
        }

        GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, previousFramebuffer));
    }

    void GLFramebuffer::attachDepthStencilTarget()
    {
        GLint previousFramebuffer;
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &previousFramebuffer);

        GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, fbo));

        // Attach color buffers if needed
        if (!colorAttachments.empty())
        {
            std::vector<GLenum> attachments;
            for (int i = 0; i < colorAttachments.size(); i++)
                attachments.push_back(GL_COLOR_ATTACHMENT0 + i);

            GL_CHECK(glDrawBuffers(static_cast<GLsizei>(attachments.size()), attachments.data()));
        }

        GL_CHECK(glGenRenderbuffers(1, &depthStencilRenderbuffer));
        GL_CHECK(glBindRenderbuffer(GL_RENDERBUFFER, depthStencilRenderbuffer));

        // Use GL_DEPTH_COMPONENT24 for depth only, or GL_DEPTH24_STENCIL8 for depth-stencil
        GL_CHECK(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height));
        GL_CHECK(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthStencilRenderbuffer));

        // Unbind renderbuffer and restore previous framebuffer
        GL_CHECK(glBindRenderbuffer(GL_RENDERBUFFER, 0));
        GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, previousFramebuffer));
    }


    void GLFramebuffer::SetNumColorAttachments(int numAttachments)
    {
        if (numAttachments <= 0)
            return;

        while (colorAttachments.size() > numAttachments) {
            colorAttachments.pop_back();
            clearColors.pop_back();
        }
        while (colorAttachments.size() < numAttachments) {
            colorAttachments.push_back(nullptr);
            clearColors.push_back(glm::vec4(0.f));
        }
    }

    void GLFramebuffer::initializeDrawBuffers()
    {
        GLint previousFramebuffer;
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &previousFramebuffer);

        GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, fbo));

        // Attach color buffers if needed
        if (!colorAttachments.empty())
        {
            std::vector<GLenum> attachments;
            for (int i = 0; i < colorAttachments.size(); i++)
                attachments.push_back(GL_COLOR_ATTACHMENT0 + i);

            GL_CHECK(glDrawBuffers(static_cast<GLsizei>(attachments.size()), attachments.data()));
        }

        GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, previousFramebuffer));
    }

    bool GLFramebuffer::isComplete()
    {
        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        switch (status)
        {
        case GL_FRAMEBUFFER_COMPLETE:
            return true;

        case GL_FRAMEBUFFER_UNDEFINED:
            TS_CORE_ERROR("Framebuffer is undefined (GL_FRAMEBUFFER_UNDEFINED).");
            break;

        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            TS_CORE_ERROR("Framebuffer has incomplete attachment (GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT).");
            break;

        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            TS_CORE_ERROR("Framebuffer is missing an attachment (GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT).");
            break;

        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
            TS_CORE_ERROR("Framebuffer has incomplete draw buffer (GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER).");
            break;

        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
            TS_CORE_ERROR("Framebuffer has incomplete read buffer (GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER).");
            break;

        case GL_FRAMEBUFFER_UNSUPPORTED:
            TS_CORE_ERROR("Framebuffer format is unsupported (GL_FRAMEBUFFER_UNSUPPORTED).");
            break;

        case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
            TS_CORE_ERROR("Framebuffer has incomplete multisample configuration (GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE).");
            break;

        case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
            TS_CORE_ERROR("Framebuffer has incomplete layer targets (GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS).");
            break;

        default:
            TS_CORE_ERROR("Framebuffer is incomplete due to an unknown error (status: {}).", std::to_string(status));
            break;
        }

        return false;
    }


    std::shared_ptr<Texture2D> GLFramebuffer::getTexture(int attachmentIndex) const
    {
        if (attachmentIndex < 0 || attachmentIndex >= colorAttachments.size())
            return nullptr;

        return colorAttachments[attachmentIndex];
    }

    std::shared_ptr<Texture2D> GLFramebuffer::getDepthStencilTexture() const
    {
        return depthStencilAttachment;
    }
    void GLFramebuffer::clear()
    {
        for (int i = 0; i < colorAttachments.size(); i++)
        {
            glm::vec4& color = clearColors[i];
            colorAttachments[i]->clear(color.r, color.g, color.b, color.a);
        }
    }
}
