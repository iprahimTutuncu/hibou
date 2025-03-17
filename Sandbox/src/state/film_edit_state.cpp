#include "film_edit_state.h"

#include <glm/glm.hpp>

#include <thsan/Input/control_setting.h>
#include <thsan/game.h>
#include <thsan/graphics/compute_shader.h>
#include <thsan/graphics/drawable/cube.h>
#include <thsan/graphics/drawable/sprite_animation.h>
#include <thsan/graphics/drawable/text.h>
#include <thsan/graphics/drawable/tilemap.h>
#include <thsan/graphics/font.h>
#include <thsan/graphics/framebuffer.h>
#include <thsan/graphics/mesh.h>
#include <thsan/graphics/modifier/on_terrain_modifier.h>
#include <thsan/graphics/render_command.h>
#include <thsan/graphics/render_states.h>
#include <thsan/graphics/shader.h>
#include <thsan/graphics/terrain/terrain.h>
#include <thsan/graphics/texture.h>
#include <thsan/graphics/view.h>
#include <thsan/hud/debug_hud/exemple_panel_debug.h>
#include <thsan/hud/debug_hud/film_editor/film_menu_bar_panel_debug.h>
#include <thsan/hud/debug_hud/film_editor/selection_panel_debug.h>
#include <thsan/hud/debug_hud/film_editor/editor_panel_debug.h>
#include <thsan/hud/debug_hud/timeline_panel_debug.h>
#include <thsan/hud/debug_hud/view_panel_debug.h>
#include <thsan/hud/general_hud/gui.h>
#include <thsan/hud/general_hud/label.h>
#include <thsan/hud/general_hud/label_button.h>
#include <thsan/hud/general_hud/panel.h>
#include <thsan/log.h>
#include <thsan/ressource_manager/font_manager.h>
#include <thsan/ressource_manager/texture2D_manager.h>
#include <thsan/graphics/vertex.h>

#include <tsm/math/transform.h>

Thsan::Selected selection;

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
	parent->addDebugPanel<ts::FilmMenuBarPanelDebug>("menubar", &parent->options, &keyframes, &frames, &activeFrame);
	parent->addDebugPanel<ts::TimelinePanelDebug>("timeline", &parent->options);
	parent->addDebugPanel<ts::SelectionPanelDebug>("selection", &parent->options, &activeFrame, &selection);
	parent->addDebugPanel<ts::EditorPanelDebug>("DataEditor", &parent->options, &selection);
    //Frame* activeFrame
    parent->addDebugPanel<ts::ViewPanelDebug>("view", &parent->options, &activeFrame);

    raytraceCS = ts::ComputeShader::create("media/compute_shader/traceFilm.cs");

    /*
    applyRayTracing(frames[1]);

    activeFrame = frames[1];

    activeFrame->getLight(0)->isValid = true;
    activeFrame->setLightCount(activeFrame->getLightCount() + 1);

    sprite = Thsan::SpriteAnimation::create();
    sprite->setTexture(activeFrame->getTexture(ts::FrameTextureType::RayTracedResult));

    int w = activeFrame->getTexture(ts::FrameTextureType::RayTracedResult)->getWidth();
    int h = activeFrame->getTexture(ts::FrameTextureType::RayTracedResult)->getHeight();


    sprite->setCurrAnimation("viewport");
    sprite->add(glm::vec4(0.f, 0.f, w, h), 0.1f);
    sprite->setSpeed(1.f);

    sprite->start();

    sprite->setPosition(glm::vec3(400.f, 200.f, 0.f));
    sprite->update(0.1f);
    */
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
    //sprite->update(deltaTime);
}

void FilmEditState::onDraw(ts::Renderer2D& target, const float& deltaTime)
{
    target.clear();
    target.display();
}

void FilmEditState::onDraw(ts::RendererTerrain& target, const float& deltaTime)
{
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

void FilmEditState::applyRayTracing(std::shared_ptr<ts::Frame> frame)
{
    if (!frame || !frame->getTexture(Thsan::FrameTextureType::RayTracedResult))
        return;

    auto rayTracedResultTexture = frame->getTexture(Thsan::FrameTextureType::RayTracedResult);
    rayTracedResultTexture->clear(0.f, 0.f, 0.f, 0.f);

    // this should be done else where
    /*
    auto positionTexture = frame->getTexture(Thsan::FrameTextureType::Position);
    auto diffuseTexture = frame->getTexture(Thsan::FrameTextureType::Normal); 
    if (!positionTexture || !diffuseTexture)
        return;

    if (!raytraceCS)
        return;

    // Bind textures to the compute shader
    raytraceCS->use();
    raytraceCS->setImage2D(positionTexture, 0); 
    raytraceCS->setImage2D(diffuseTexture, 1);
    raytraceCS->setImage2D(diffuseTexture, 2);
    raytraceCS->setRWImage2D(rayTracedResultTexture, 3);

    // Pass camera intrinsics and screen size
    raytraceCS->setVec2("screenSize", glm::vec2(diffuseTexture->getWidth(), diffuseTexture->getHeight()));
    raytraceCS->setVec4("cameraIntrinsics", glm::vec4(676.f / 2.f, 308.f / 2.f, 676.f / 2.f, 400.f));

    // Dispatch compute shader
    unsigned int workgroupSize = 16;
    unsigned int dispatchX = (diffuseTexture->getWidth() + workgroupSize - 1) / workgroupSize;
    unsigned int dispatchY = (diffuseTexture->getHeight() + workgroupSize - 1) / workgroupSize;

    raytraceCS->dispatch(dispatchX, dispatchY, 1);
    raytraceCS->wait();
    */
}

