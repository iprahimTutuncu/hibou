#include "film_edit_state.h"
#include <thsan/log.h>
#include <glm/glm.hpp>
#include <thsan/Input/control_setting.h>
#include <thsan/graphics/vertex.h>
#include <thsan/graphics/mesh.h>
#include <thsan/graphics/shader.h>
#include <thsan/graphics/texture.h>
#include <thsan/graphics/framebuffer.h> 
#include <thsan/graphics/render_command.h>
#include <thsan/graphics/render_states.h>
#include <thsan/graphics/drawable/tilemap.h>
#include <thsan/graphics/terrain/terrain.h>
#include <thsan/graphics/view.h>
#include <thsan/graphics/drawable/sprite_animation.h>
#include <thsan/graphics/compute_shader.h>
#include <thsan/graphics/modifier/on_terrain_modifier.h>
#include <thsan/game.h>
#include <tsm/math/transform.h>
#include <thsan/hud/debug_hud/exemple_panel_debug.h>
#include <thsan/hud/debug_hud/menu_bar_panel_debug.h>
#include <thsan/graphics/drawable/cube.h>
#include <thsan/graphics/font.h>
#include <thsan/graphics/drawable/text.h>
#include <thsan/ressource_manager/font_manager.h>
#include <thsan/hud/general_hud/label_button.h>
#include <thsan/hud/general_hud/label.h>
#include <thsan/hud/general_hud/gui.h>
#include <thsan/hud/general_hud/panel.h>
#include <thsan/graphics/texture.h>
#include "../../../Thsan/include/thsan/ressource_manager/texture2D_manager.h"

FilmEditState::FilmEditState(ts::Game* parent):
	ts::State(parent)
{
	parent->options.renderOptions.enableRendererTerrain = 0;
}

void FilmEditState::onInit()
{
}

void FilmEditState::onStart()
{
	parent->enableDebugUI();
	parent->addDebugPanel<ts::MenuBarPanelDebug>("menubar", &parent->options);

    raytraceCS = ts::ComputeShader::create("media/compute_shader/traceFilm.cs");

    importFrame("media/image/film/shadow", 1);
    applyRayTracing(frames[1]);


    sprite = Thsan::SpriteAnimation::create();
    sprite->setTexture(frames[1].rayTracedResult);
    int w = frames[1].rayTracedResult->getWidth();
    int h = frames[1].rayTracedResult->getHeight();
    sprite->setCurrAnimation("viewport");
    sprite->add(glm::vec4(0.f, 0.f, w, h), 0.1f);
    sprite->setSpeed(1.f);

    sprite->start();

    sprite->setPosition(glm::vec3(400.f, 200.f, 0.f));
    sprite->update(0.1f);
}

void FilmEditState::onExit()
{
}

void FilmEditState::onSuspend()
{
}

void FilmEditState::onResume()
{
}

void FilmEditState::onInput(const float& deltaTime, std::vector<ts::InputAction> inputActions)
{
}

void FilmEditState::onUpdate(const float& deltaTime)
{
    sprite->update(deltaTime);
}

void FilmEditState::onDraw(ts::Renderer2D& target, const float& deltaTime)
{
    static bool first = true;

    if (first)
    {
        target.add(sprite);
        first = false;
    }

    target.display();
}

void FilmEditState::onDraw(ts::RendererTerrain& target, const float& deltaTime)
{
}


bool FilmEditState::importFrame(std::string frame_path, int index)
{
    if (index < 0)
        return false;

    Frame frame;
    frame.diffuse = ts::Texture2D::create();
    frame.positionMap = ts::Texture2D::create();

    if (!frame.diffuse->loadFromFile(frame_path + "_diffuse.png", false) || !frame.positionMap->loadFromFile(frame_path + "_position.png", false))
    {
        return false;
    }

    frame.rayTracedResult = ts::Texture2D::create();
    frame.rayTracedResult->generate(glm::uvec2(frame.diffuse->getWidth(), frame.diffuse->getHeight()), Thsan::TextureFormat::RGBA32F);


    if (index >= frames.size())
    {
        frames.resize(index + 1);
        totalFrames = frames.size();
    }

    frames[index] = std::move(frame);
    return true;
}

bool FilmEditState::exportFrame(std::string frame_path)
{
    if (currentFrame < 0 || currentFrame >= frames.size())
        return false;

    Frame& frame = frames[currentFrame];

    if (!frame.rayTracedResult)
        return false;

    return false;
   // return frame.rayTracedResult->saveToFile(frame_path + "_raytraced.png");
}

void FilmEditState::setKeyframe(int frameIndex)
{
    if (frameIndex < 0 || frameIndex >= frames.size())
        return;

    keyframes[frameIndex] = frames[frameIndex];
}

void FilmEditState::removeKeyframe(int frameIndex)
{
    keyframes.erase(frameIndex);
}

void FilmEditState::applyRayTracing(Frame& frame)
{
    frame.rayTracedResult->clear(0.f, 0.f, 0.f, 0.f);

    if (!raytraceCS || !frame.diffuse || !frame.positionMap || !frame.rayTracedResult)
        return;

    // Bind textures to the compute shader
    raytraceCS->use();
    raytraceCS->setImage2D(frame.positionMap, 0);
    raytraceCS->setImage2D(frame.diffuse, 1);
    raytraceCS->setImage2D(frame.diffuse, 2);
    raytraceCS->setRWImage2D(frame.rayTracedResult, 3);
    raytraceCS->setVec2("screenSize", glm::vec2(frame.diffuse->getWidth(), frame.diffuse->getHeight()));
    raytraceCS->setVec4("cameraIntrinsics", glm::vec4(676.f/2.f, 308.f/2.f, 676.f/2.f, 400.f));

    // Dispatch compute shader
    unsigned int workgroupSize = 16;
    unsigned int dispatchX = (frame.diffuse->getWidth() + workgroupSize - 1) / workgroupSize;
    unsigned int dispatchY = (frame.diffuse->getHeight() + workgroupSize - 1) / workgroupSize;

    raytraceCS->dispatch(dispatchX, dispatchY, 1);
    raytraceCS->wait();
}
