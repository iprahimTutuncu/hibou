#include "pch.h"
#include "render_context.h"
#include "render_states.h"
#include "render_target.h"
#include "render_command.h"
#include "framebuffer.h"
#include <thsan/log.h>
#include <GL/glew.h>
#include <platform/openGL/gl_helper.h>

bool Thsan::RenderContext::init()
{
    target = Thsan::create_renderTarget();
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

