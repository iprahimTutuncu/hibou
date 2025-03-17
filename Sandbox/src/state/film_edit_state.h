#pragma once
#include <thsan/state/state.h>
#include <thsan/graphics/terrain/terrain.h>
#include <thsan/graphics/light.h>
#include <thsan/graphics/frame.h>
#include <thsan/graphics/layer.h>

#define ts Thsan

namespace ts 
{
	class Shader;
	class Texture2D;
	class Framebuffer;
	class RenderStates;
	class View;
	class SpriteAnimation;
	class ComputeShader;
	struct Material;
}

class FilmEditState : public ts::State {
public:
	FilmEditState(ts::Game* parent);
	~FilmEditState() = default;

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
	void setKeyframe(int frameIndex);
	void removeKeyframe(int frameIndex);

	std::map<int, std::shared_ptr<ts::Frame>> keyframes;
	std::vector<std::shared_ptr<ts::Frame>> frames;

	std::shared_ptr<ts::Frame> activeFrame;

	int currentFrame{ -1 };
	int totalFrames{ 0 };

	glm::vec3 positioOnTerrain;

	std::shared_ptr<ts::Texture2D> screen_texture;
	std::shared_ptr<ts::SpriteAnimation> sprite;
	std::shared_ptr<ts::ComputeShader> raytraceCS;

	void applyRayTracing(std::shared_ptr<ts::Frame> frame);
};