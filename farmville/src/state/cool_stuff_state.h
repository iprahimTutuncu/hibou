#pragma once
#include <thsan/state/State.h>
#include <thsan/graphics/terrain/terrain.h>

#define ts Thsan
#define ui Thsan::UI

namespace ui 
{
	class Gui;
	class LabelButton;
	class Panel;
}

namespace ts {
	class Shader;
	class Texture2D;
	class Framebuffer;
	class RenderStates;
	class Tilemap;
	class View;
	class SpriteAnimation;
	class Terrain;
	class ComputeShader;
	class OnTerrainModifier;
	class Text;
}

namespace tsm {
	class AbstractCamera;
	class Transform;
}


class CoolStuffState : public ts::State {
public:
	CoolStuffState(ts::Game* parent);
	~CoolStuffState() = default;

	// Inherited via State
	virtual void onInit() override;
	virtual void onStart() override;
	virtual void onExit() override;
	virtual void onSuspend() override;
	virtual void onResume() override;

	virtual void onInput(const float& deltaTime, std::vector<ts::InputAction> inputActions) override;
	virtual void onUpdate(const float& deltaTime) override;
	virtual void onDraw(ts::Renderer2D& target, const float& deltaTime) override;
	virtual void onDraw(ts::RendererTerrain& target, const float& deltaTime) override;

private:
	glm::vec3 positioOnTerrain;

	std::shared_ptr<ts::Shader> shader;
	std::shared_ptr<ts::Text> text;
	std::shared_ptr<ts::Texture2D> sprite_texture;
	std::shared_ptr<ts::Texture2D> tilemap_texture;
	std::shared_ptr<ts::Tilemap> tilemap;
	std::shared_ptr<ts::View> view;
	std::shared_ptr<ts::SpriteAnimation> spriteAnimation;
	std::shared_ptr<ts::ComputeShader> computeShaderTest;
	std::shared_ptr<ui::Gui> gui;
	std::shared_ptr<ui::Panel> panel;
	std::shared_ptr<ui::LabelButton> bttnPlay;
	std::shared_ptr<ui::LabelButton> bttnQuit;
	std::shared_ptr<tsm::AbstractCamera> freeCamera;
	std::shared_ptr<tsm::Transform> transform;

	ts::TerrainOptions* terrainOptions;
};