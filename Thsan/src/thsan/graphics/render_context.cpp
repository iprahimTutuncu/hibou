#include "pch.h"
#include "thsan/graphics/render_context.h"
#include "thsan/graphics/render_states.h"
#include "thsan/graphics/render_target.h"
#include "thsan/graphics/render_command.h"
#include "thsan/graphics/framebuffer.h"
#include <thsan/log.h>
#include <GL/glew.h>
#include <platform/openGL/gl_helper.h>


namespace Thsan
{
    int RenderContext::x{ 0 };
    int RenderContext::y{ 0 };
    int RenderContext::w{ 800 };
    int RenderContext::h{ 600 };

    std::queue<std::unique_ptr<RenderCmd::Command>> RenderContext::commandList;
    std::weak_ptr<Framebuffer> RenderContext::curr_fb;
    std::weak_ptr<RenderStates> RenderContext::curr_state;
    std::shared_ptr<RenderTarget> RenderContext::target = Thsan::RenderTarget::create();
}

bool Thsan::RenderContext::init()
{
    target->init();
    return target != nullptr;
}

void Thsan::RenderContext::close()
{
    while (commandList.size() > 0)
        commandList.pop();
}

void Thsan::RenderContext::clear()
{
    target->clear();
}

void Thsan::RenderContext::setClearColor(glm::vec4 color)
{
    target->setClearColor(color.r, color.g, color.b, color.a);
}

void Thsan::RenderContext::setViewport(int x, int y, int w, int h)
{
    target->setViewport(x, y, w, h);
}

void Thsan::RenderContext::setDefaultViewport(int x, int y, int w, int h)
{
    // lol
}

void Thsan::RenderContext::setState(std::shared_ptr<RenderStates> state)
{
    curr_state = state;
    auto s = curr_state.lock();
    if (s)
        s->bind();
}

void Thsan::RenderContext::beginPass(std::shared_ptr<Framebuffer> fb)
{
    if (fb) {
        curr_fb = fb;
        fb->bind();
    }
}

void Thsan::RenderContext::submit(std::unique_ptr<RenderCmd::Command> command)
{
    commandList.push(std::move(command));
}

void Thsan::RenderContext::endPass()
{
    flush();

    auto fbo = curr_fb.lock();
    if (fbo)
        fbo->Unbind();
}

void Thsan::RenderContext::flush()
{
    while (!commandList.empty())
    {
        commandList.front()->execute(target);
        commandList.pop();
    }
}
