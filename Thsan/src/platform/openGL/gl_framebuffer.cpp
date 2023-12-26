#include "pch.h"
#include "thsan/graphics/texture.h"
#include "thsan/log.h"
#include "gl_framebuffer.h"
#include "GL/glew.h"
#include "gl_helper.h"


namespace Thsan {

    const std::unordered_map<TextureFormat, std::tuple<GLenum, GLenum, GLenum>> FormatMapping = {
    {TextureFormat::RGBA8, {GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE}},
    {TextureFormat::R32F, {GL_R32F, GL_RED, GL_FLOAT}},
    {TextureFormat::RGB16F, {GL_RGB16F, GL_RGB, GL_HALF_FLOAT}},
    // Add more mappings as needed
    };

    GLFramebuffer::GLFramebuffer() :
        Framebuffer(0, 0),
        fbo(0)
    {
    }

    GLFramebuffer::GLFramebuffer(uint32_t width, uint32_t height) :
        Framebuffer(width, height)
    {
        GL_CHECK(glGenFramebuffers(1, &fbo));
        GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, fbo));
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
        if (attachmentIndex >= colorAttachments.size()) {
            TS_CORE_ERROR("in Framebuffer::attachColorTarget, attachmentIndex is {} which is bigger than the numColorAttachments that is {}, so aborted buddy\n", attachmentIndex, colorAttachments.size());
            return;
        }
        auto it = FormatMapping.find(format);
        if (it != FormatMapping.end()) {
            auto [internalFormat, glFormat, glType] = it->second;

            std::stringstream ss;
            ss << "fbo" << fbo;
            ss << "_attachmentIndex" << attachmentIndex;
            ss << "_internalFormat" << internalFormat;
            ss << "_format" << glFormat;
            ss << "_type" << glType;
            std::string name = ss.str();

            auto texture = RessourceManager::Texture2DManager::create(name, { size, internalFormat, glFormat, glType, nullptr });

            GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachmentIndex, GL_TEXTURE_2D, texture->getID(), 0));

            colorAttachments[attachmentIndex] = texture;
        }
    }

    void GLFramebuffer::attachDepthStencilTarget()
    {
        GLint previousFramebuffer;
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &previousFramebuffer);

        GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, fbo));

        std::vector<GLenum> attachments;
        for (int i = 0; i < colorAttachments.size(); i++)
            attachments.push_back(GL_COLOR_ATTACHMENT0 + i);

        GL_CHECK(glDrawBuffers(attachments.size(), attachments.data()));

        GL_CHECK(glGenRenderbuffers(1, &depthStencilRenderbuffer));
        GL_CHECK(glBindRenderbuffer(GL_RENDERBUFFER, depthStencilRenderbuffer));

        GL_CHECK(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, size.x, size.y));
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
        return std::shared_ptr<Texture2D>();
    }

    std::shared_ptr<Texture2D> GLFramebuffer::getDepthTexture() const
    {
        return std::shared_ptr<Texture2D>();
    }
