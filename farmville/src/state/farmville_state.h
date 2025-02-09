#pragma once
#include <thsan/state/State.h>
#include <thsan/graphics/terrain/terrain.h>

#define ts Thsan
#define tsui Thsan::UI

#include "core/scene.h"

namespace tsui
{
	class Gui;
	class LabelButton;
	class Panel;
}

namespace ts
{
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

namespace tsm
{
	class AbstractCamera;
	class Transform;
}


class FarmVilleState : public ts::State {
public:
	FarmVilleState(ts::Game* parent);
	~FarmVilleState() = default;

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
	std::shared_ptr<ts::Text> text;
	std::shared_ptr<ts::View> view;
	std::shared_ptr<tsm::AbstractCamera> freeCamera;

	ts::TerrainOptions* terrainOptions;
	Scene scene;

	GameObject* player;
};