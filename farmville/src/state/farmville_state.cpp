#include "farmville_state.h"
#include <thsan/game.h>
#include <thsan/Input/control_setting.h>
#include <thsan/graphics/view.h>
#include <thsan/graphics/drawable/sprite_animation.h>

#include "game_object/player/player_input_component.h"
#include "game_object/player/player_physic_component.h"
#include "game_object/player/player_sprite_render_component.h"
#include "game_object/camera/camera_render_component.h"
#include <thsan/hud/debug_hud/menu_bar_panel_debug.h>
#include <thsan/hud/debug_hud/terrain_panel_debug.h>

FarmVilleState::FarmVilleState(ts::Game* parent) :
    ts::State(parent),
	scene(this)
{
}

void FarmVilleState::onInit()
{
	//make a default if not exist
	view = Thsan::View::create(0, 0, 800, 600, 300.f, 1.f, 0.f);
	view->setZoom(1.f);
	terrainOptions = &this->getParent()->options.renderOptions.terrainOptions;
	parent->disableDebugUI();

	terrainOptions->position[0] = 40.0f;
	terrainOptions->position[1] = 70.5f;
	terrainOptions->position[2] = 47.0f;

	terrainOptions->normalBlurr[0] = 0.0f;
	terrainOptions->normalBlurr[1] = 0.0f;

	// Initialize other members
	terrainOptions->terrainSize = 2048;

	terrainOptions->normalBlurIteration = 0;
	terrainOptions->LOD = 0.0f;
	terrainOptions->LODStartAt = 100.f;
	terrainOptions->angle = 0.0f;
	terrainOptions->horizon = 100.0f;
	terrainOptions->scaleHeight = 256.f;
	terrainOptions->spriteRenderDistance = 300.0f;
	terrainOptions->terrainRenderDistance = 300.0f;
	terrainOptions->shininess = 0.0f;
	terrainOptions->blurType = 0;

	strcpy_s(terrainOptions->albedoTexturePath, sizeof(terrainOptions->albedoTexturePath), "media/terrain/fissure/terrain_color.png");
	strcpy_s(terrainOptions->heightTexturePath, sizeof(terrainOptions->heightTexturePath), "media/terrain/fissure/terrain.png");
	strcpy_s(terrainOptions->collisionHeightTexturePath, sizeof(terrainOptions->heightTexturePath), "media/terrain/fissure/terrain_height.png");
	strcpy_s(terrainOptions->collisionMaskTexturePath, sizeof(terrainOptions->collisionMaskTexturePath), "media/terrain/fissure/terrain_mask.png");


	parent->add(ts::Key::A, ts::InputState::isPressed, ts::InputAction::left);
	parent->add(ts::Key::S, ts::InputState::isPressed, ts::InputAction::down);
	parent->add(ts::Key::W, ts::InputState::isPressed, ts::InputAction::up);
	parent->add(ts::Key::D, ts::InputState::isPressed, ts::InputAction::right);
	parent->add(ts::Key::P, ts::InputState::isPressed, ts::InputAction::run);
	parent->add(ts::Key::P, ts::InputState::isReleased,ts::InputAction::stop_run);
	parent->add(ts::Key::E, ts::InputState::isPressed, ts::InputAction::rotateLeft);
	parent->add(ts::Key::Q, ts::InputState::isPressed, ts::InputAction::rotateRight);
	parent->add(ts::Key::O, ts::InputState::isPressed, ts::InputAction::aim);
	parent->add(ts::Key::O, ts::InputState::isReleased,ts::InputAction::stop_aim);
	parent->add(ts::Key::Space, ts::InputState::isPressedNoRepeat, ts::InputAction::jump);
	parent->add(ts::Key::Return, ts::InputState::isPressedNoRepeat, ts::InputAction::select);
	parent->add(ts::Key::L, ts::InputState::isPressedNoRepeat, ts::InputAction::action);
	parent->add(ts::Key::K, ts::InputState::isPressedNoRepeat, ts::InputAction::attack);
}

void FarmVilleState::onStart()
{
	parent->enableDebugUI();
	parent->addDebugPanel<ts::TerrainPanelDebug>("terain", &this->getParent()->options.renderOptions.terrainOptions);
	parent->addDebugPanel<ts::MenuBarPanelDebug>("menubar", &parent->options);

	//cube = ts::Cube::create();
	//cube->setScale(glm::vec3(100.f, 100.f, 100.f));
	//cube->setTexture(tilemap_texture);

	player = scene.createGameObject("player");
	scene.setGameObjectInputComponent<PlayerInputComponent>(player);
	scene.setGameObjectPhysicComponent<PlayerPhysicComponent>(player);
	scene.setGameObjectRenderComponent<PlayerSpriteRenderComponent>(player);

	GameObject* go = scene.createGameObject("camera");
	scene.setGameObjectRenderComponent<CameraRenderComponent>(go, player);
}

void FarmVilleState::onExit()
{
    // Function is empty
}

void FarmVilleState::onSuspend()
{
    // Function is empty
}

void FarmVilleState::onResume()
{
    // Function is empty
}

void FarmVilleState::onInput(const float& deltaTime, std::vector<ts::InputAction> inputActions)
{
	scene.input(deltaTime, inputActions);
}

void FarmVilleState::onUpdate(const float& deltaTime)
{
	scene.update(deltaTime);
}

void FarmVilleState::onDraw(ts::RendererTerrain& target, const float& deltaTime)
{
	scene.render(deltaTime, target);

	target.display();
}

void FarmVilleState::onDraw(ts::Renderer2D& target, const float& deltaTime)
{
}
