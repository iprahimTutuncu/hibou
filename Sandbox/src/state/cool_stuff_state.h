#pragma once
#include <thsan/state/State.h>

#define ts Thsan

namespace ts {
	class Mesh;
	class Shader;
	class Texture2D;
	class Framebuffer;
	class RenderStates2D;
	class Tilemap;
	class View;
	class SpriteAnimation;
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
	virtual void init() override;

	virtual void input(const float& deltaTime, std::vector<ts::InputAction> inputActions) override;

	virtual void update(const float& deltaTime) override;

	virtual void draw(ts::RenderManager* target, const float& deltaTime) override;

private:
	std::shared_ptr<ts::Mesh> mesh;
	std::shared_ptr<ts::Shader> shader;
	std::shared_ptr<ts::Texture2D> sprite_texture;
	std::shared_ptr<ts::Texture2D> tilemap_texture;
	std::shared_ptr<ts::Framebuffer> framebuffer;
	std::shared_ptr<ts::RenderStates2D> renderstates;
	std::shared_ptr<ts::Tilemap> tilemap;
	std::shared_ptr<ts::View> view;
	std::shared_ptr<ts::SpriteAnimation> spriteAnimation;
	std::shared_ptr<tsm::AbstractCamera> freeCamera;
	std::shared_ptr<tsm::Transform> transform;
};