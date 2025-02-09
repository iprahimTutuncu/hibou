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
#include <thsan/hud/debug_hud/terrain_panel_debug.h>
#include <thsan/hud/debug_hud/menu_bar_panel_debug.h>
#include "cool_stuff_state.h"
#include <thsan/graphics/drawable/cube.h>
#include <thsan/graphics/font.h>
#include <thsan/graphics/drawable/text.h>
#include <thsan/ressource_manager/font_manager.h>
#include <thsan/hud/general_hud/label_button.h>
#include <thsan/hud/general_hud/label.h>
#include <thsan/hud/general_hud/gui.h>
#include <thsan/hud/general_hud/panel.h>

struct world {
	float gravity{ -9.81f };
};

struct Player{
	glm::vec2 position;
	float jump_height;
	float jump_duration;
};

float get_gravity(Player p) {
	return (-2 * p.jump_height) / (p.jump_duration * p.jump_duration);
}

float get_initial_velocity(Player p) {
	return (2 * p.jump_height) / p.jump_duration;
}

CoolStuffState::CoolStuffState(ts::Game* parent):
	ts::State(parent)
{ 
}

void CoolStuffState::onInit()
{
	//make a default if not exist
	view = Thsan::View::create(0, 0, 800, 768, 300.f, 1.f, 0.f);
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
	parent->add(ts::Key::P, ts::InputState::isReleased, ts::InputAction::stop_run);
	parent->add(ts::Key::E, ts::InputState::isPressed, ts::InputAction::rotateLeft);
	parent->add(ts::Key::Q, ts::InputState::isPressed, ts::InputAction::rotateRight);
	parent->add(ts::Key::O, ts::InputState::isPressed, ts::InputAction::aim);
	parent->add(ts::Key::O, ts::InputState::isReleased, ts::InputAction::stop_aim);
	parent->add(ts::Key::Space, ts::InputState::isPressedNoRepeat, ts::InputAction::jump);
	parent->add(ts::Key::Return, ts::InputState::isPressedNoRepeat, ts::InputAction::select);
	parent->add(ts::Key::L, ts::InputState::isPressedNoRepeat, ts::InputAction::action);
	parent->add(ts::Key::K, ts::InputState::isPressedNoRepeat, ts::InputAction::attack);
}

void CoolStuffState::onStart()
{

	terrainOptions = &this->getParent()->options.renderOptions.terrainOptions;
	parent->enableDebugUI();
	parent->addDebugPanel<ts::TerrainPanelDebug>("terain", &this->getParent()->options.renderOptions.terrainOptions);
	parent->addDebugPanel<ts::MenuBarPanelDebug>("menubar", &parent->options);

	positioOnTerrain.y = 0;
	positioOnTerrain.z = 0;
	positioOnTerrain.x = 0;

	parent->add(ts::Key::Left, ts::InputState::isPressed, ts::InputAction::left);
	parent->add(ts::Key::Right, ts::InputState::isPressed, ts::InputAction::right);
	parent->add(ts::Key::Up, ts::InputState::isPressed, ts::InputAction::up);
	parent->add(ts::Key::Down, ts::InputState::isPressed, ts::InputAction::down);
	parent->add(ts::Key::Q, ts::InputState::isPressed, ts::InputAction::rotateLeft);
	parent->add(ts::Key::E, ts::InputState::isPressed, ts::InputAction::rotateRight);
	parent->add(ts::Key::Z, ts::InputState::isPressed, ts::InputAction::action);
	parent->add(ts::Key::Lshift, ts::InputState::isPressed, ts::InputAction::select);

	gui = ui::Gui::create();
	gui->setScreenSize(this->getParent()->options.windowOptions.screenWidth, this->getParent()->options.windowOptions.screenHeight);

	std::shared_ptr<Thsan::Font> font = Thsan::Font::create("media/font/font1.fnt", "media/font/font1.png");
	auto title = ui::Label::create("Label-0");
	bttnPlay = ui::LabelButton::create("LabelButton-0");
	bttnQuit = ui::LabelButton::create("LabelButton-1");
	title->setMargin(glm::vec2(150, 100));
	title->setSize(glm::vec2(300, 32));	
	title->setColor(tsm::Color(128, 128, 255, 255));

	bttnPlay->setMargin(glm::vec2(300, 300));
	bttnPlay->setSize(glm::vec2(200, 64));	

	bttnPlay->setColor(tsm::Color(1.0, 0.0, 0.5, 255));
	bttnPlay->setString(U"play");

	bttnPlay->connect(ui::Action::hover, [&]() {
		bttnPlay->setColor(tsm::Color(0.0, 0.0, 255, 255));
		});

	bttnPlay->connect(ui::Action::crossed, [&]() {
		bttnPlay->setColor(tsm::Color(255, 255, 255, 255));
		bttnPlay->setString(U"PLAY");
		});

	//bttn quit
	bttnQuit->setMargin(glm::vec2(300, 450));
	bttnQuit->setSize(glm::vec2(200, 64));

	bttnQuit->setColor(tsm::Color(1.0, 0.0, 0.5, 255));
	bttnQuit->setString(U"quit");

	bttnQuit->connect(ui::Action::hover, [&]() {
		bttnQuit->setColor(tsm::Color(0.0, 55.0, 255, 255));
		});

	bttnQuit->connect(ui::Action::crossed, [&]() {
		bttnQuit->setColor(tsm::Color(255, 255, 255, 255));
		bttnQuit->setString(U"quit");
		});

	bttnQuit->set(ui::Direction::up, bttnPlay);
	bttnQuit->set(ui::Direction::down, bttnPlay);

	bttnPlay->set(ui::Direction::up, bttnQuit);
	bttnPlay->set(ui::Direction::down, bttnQuit);

	panel = ui::Panel::create();

	ui::PanelData pd;
	pd.backgroundColor = tsm::Color(255, 0, 0, 255);
	pd.borderWidth = 10.f;
	pd.colorBorder1 = tsm::Color(135, 244, 239, 255);
	pd.colorBorder2 = tsm::Color(231, 230, 179, 255);
	pd.colorBorder3 = tsm::Color(76, 50, 76, 255);
	pd.cornerRadius = 7.f;
	pd.enableBgTexture = true;
	pd.shadowSize = 0.f;

	panel->setBgTexture("media/image/sasuke.jpg");
	panel->setShaderParams(pd);
	panel->setMargin(glm::vec2(100, 100));
	panel->setSize(glm::vec2(700, 500));

	gui->setFont(font);

	gui->add(panel);
	gui->add(title);
	gui->add(bttnPlay);
	gui->add(bttnQuit);

	gui->setSelection(bttnPlay);

	title->setColor(tsm::Color(128, 128, 255, 255));
	title->setString(U"nicest game");

	sprite_texture = parent->getTexture("media/image/sonic.png");

	spriteAnimation = Thsan::SpriteAnimation::create();
	spriteAnimation->setTexture(sprite_texture);
	spriteAnimation->setCurrAnimation("idle");
	spriteAnimation->add(glm::vec4(11.f, 14.f, 22.f, 32.f), 0.1f);
	spriteAnimation->add(glm::vec4(38.f, 12.f, 22.f, 34.f), 0.1f);
	spriteAnimation->add(glm::vec4(65.f, 11.f, 24.f, 35.f), 0.1f);
	spriteAnimation->add(glm::vec4(94.f, 11.f, 24.f, 35.f), 0.1f);
	spriteAnimation->add(glm::vec4(123.f, 14.f, 26.f, 32.f), 0.1f);
	spriteAnimation->add(glm::vec4(154.f, 15.f, 24.f, 31.f), 0.1f);
	spriteAnimation->enableLoop();
	spriteAnimation->setSpeed(1.f);

	spriteAnimation->setKeyColor(tsm::Color(0, 112, 112, 255));
	spriteAnimation->start();

	tilemap = Thsan::Tilemap::create();
	const int tiles[] =
	{ 
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,240,241,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,328,329,0,0,0,0,0,0,0,0,0,566,567,568,567,568,569,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,328,329,0,0,0,0,0,0,0,0,0,610,611,612,611,612,613,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,566,567,568,567,568,568,567,568,568,568,567,568,569,0,0,654,655,656,656,656,657,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,610,611,612,611,611,611,612,611,612,611,611,612,613,0,0,0,0,0,0,0,0,0,0,0,0,566,567,568,569,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,610,611,612,611,612,611,612,611,612,611,611,612,613,0,0,0,0,0,0,0,0,0,0,0,0,610,611,612,613,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,610,611,612,611,612,611,612,611,612,611,611,612,613,0,0,0,0,0,0,0,0,0,0,0,0,654,655,656,657,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,610,611,612,611,612,611,612,611,612,611,611,612,613,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,610,611,612,611,612,611,612,611,612,611,611,612,613,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,610,611,612,611,612,611,612,611,612,611,611,612,613,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,610,611,612,611,612,611,612,611,612,611,611,612,613,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,610,611,612,611,612,611,612,611,612,611,611,612,613,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,610,611,611,611,612,611,611,611,612,611,611,612,613,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	};

	tilemap_texture = parent->getTexture("media/image/tileset.png");

	Thsan::Tileset tileset;
	tileset.margin = 8;
	tileset.tilewidth = 16;
	tileset.tileheight = 16;
	tileset.tilemapWidth = 50;
	tileset.tilemapHeight = 40;
	tileset.backgroundcolor = tsm::Color(0, 0, 0, 0);
	tileset.spacing = 0;

	tilemap->loadTileset(tileset, tilemap_texture);
	tilemap->load(tiles);
	
	//make a default if not exist
	view = Thsan::View::create(0, 0, 1280, 768, 300.f, 1.f, 0.f);
	view->setZoom(1.f);

	//cube = ts::Cube::create();
	//cube->setScale(glm::vec3(100.f, 100.f, 100.f));
	//cube->setTexture(tilemap_texture);

	terrainOptions->position[0] = 0.0f;
	terrainOptions->position[1] = 190.5f;
	terrainOptions->position[2] = 70.0f;

	terrainOptions->normalBlurr[0] = 0.0f;
	terrainOptions->normalBlurr[1] = 0.0f;

	// Initialize other members
	terrainOptions->terrainSize = 1024;

	terrainOptions->normalBlurIteration = 0;
	terrainOptions->LOD = 0.0f;
	terrainOptions->LODStartAt = 0.0f;
	terrainOptions->angle = 0.0f;
	terrainOptions->horizon = 50.0f;
	terrainOptions->scaleHeight = 256.f;
	terrainOptions->spriteRenderDistance = 300.0f;
	terrainOptions->terrainRenderDistance = 1000.0f;
	terrainOptions->shininess = 0.0f;
	terrainOptions->blurType = 0;

	strcpy_s(terrainOptions->albedoTexturePath, sizeof(terrainOptions->albedoTexturePath), "media/terrain/C14.png");
	strcpy_s(terrainOptions->heightTexturePath, sizeof(terrainOptions->heightTexturePath), "media/terrain/D14.png");
	strcpy_s(terrainOptions->collisionHeightTexturePath, sizeof(terrainOptions->heightTexturePath), "media/terrain/D14.png");
	strcpy_s(terrainOptions->collisionMaskTexturePath, sizeof(terrainOptions->collisionMaskTexturePath), "media/terrain/D14.png");
	
	//yo low-key forgot why
	spriteAnimation->update(0.1f);

	text = Thsan::Text::create();
	text->setFont(font);

	text->setColor(tsm::Color(255, 0, 255));
	text->setString(U"abcdefghijklmnopqrstuvwxyzéèâêô,.é;`^¸?!/$%?&*()! 0 123456789-=ABCDEFGHIJKLMNOPQRSTUVWXYZ");

	text->setScale(glm::vec3(0.75f, 2.0, 0.f));
	text->setPosition(glm::vec3(50.f, 50.f, 0.f));
	text->rotate(glm::vec3(0.f, 0.f, 1.f), 45.f);
}

void CoolStuffState::onExit() 
{

}

void CoolStuffState::onSuspend() 
{

}

void CoolStuffState::onResume()
{

}

void CoolStuffState::onInput(const float& deltaTime, std::vector<ts::InputAction> inputActions)
{
	gui->handleEvent(inputActions);

	static float x = 0.f;
	static float y = 0.f;
	static float r = 0.f;
	static float t = 0.f;
	static float z = 1.f;

	view->setZoom(z);
	view->setRotation(r);
	view->setPosition(0, 0);

	for (ts::InputAction i : inputActions) {
		if (i == Thsan::InputAction::left)
			positioOnTerrain.x += 1.5f;

		if (i == Thsan::InputAction::right)
			positioOnTerrain.x -= 1.5f;

		if (i == Thsan::InputAction::up) {
			positioOnTerrain.y += 1.5f;
		}
		if (i == Thsan::InputAction::down)
			positioOnTerrain.y -= 1.5f;

		if (i == Thsan::InputAction::rotateLeft)
			positioOnTerrain.z += 1.5;
		if (i == Thsan::InputAction::rotateRight)
			positioOnTerrain.z -= 1.5;
		if (i == Thsan::InputAction::action) {
			terrainOptions->scaleHeight += 1.1f;
		}
		if (i == Thsan::InputAction::select)
			terrainOptions->scaleHeight -= 1.1f;
	}

	t += 10.f * deltaTime;
}

void CoolStuffState::onUpdate(const float& deltaTime)
{
	static float t = 0.f;
	t += deltaTime;
	spriteAnimation->update(deltaTime);
	//terrainOptions->LOD = 0.0f;
	//terrainOptions->LODStartAt = 0.0f;
	float y = spriteAnimation->getFrameHeight()/2.f;
	float x = spriteAnimation->getFrameWidth()/2.f;

	spriteAnimation->setExternalTransform(positioOnTerrain, glm::vec2(1.f, 1.f), 45.f);

	gui->update(deltaTime);
}

void CoolStuffState::onDraw(ts::RendererTerrain& target, const float& deltaTime)
{
	static bool first = true;

	if (first)
	{
		target.add(spriteAnimation);
		first = false;
	}
	target.display();
}

void CoolStuffState::onDraw(ts::Renderer2D& target, const float& deltaTime)
{
	bool first_time = true;
	if (first_time)
	{
		target.setGui(gui);
		first_time = false;
	}
	target.setView(view);

	target.add(tilemap);
	target.add(text);
	target.display();
	target.remove(tilemap);
	target.remove(text);
}
