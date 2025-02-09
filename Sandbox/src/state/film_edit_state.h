#pragma once
#include <thsan/state/state.h>
#include <thsan/graphics/terrain/terrain.h>
#include <thsan/graphics/light.h>

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

	struct Frame
	{
		std::shared_ptr<ts::Texture2D> diffuse;
		std::shared_ptr<ts::Texture2D> positionMap;
		std::shared_ptr<ts::Texture2D> rayTracedResult;
	};

	struct Light
	{
		bool isEnable;
		bool isDirection;
		bool isPoint;
		bool isSpot;
		bool isCursor;

		glm::vec3 position;
		glm::vec3 direction;

		float attenuationConstant;
		float attenuationLinear;
		float attenuationQuadratic;

		tsm::Color ambient;
		tsm::Color diffuse;
		tsm::Color specular;
	};

	bool importFrame(std::string frame_path, int index);
	bool exportFrame(std::string frame_path);

	void setKeyframe(int frameIndex);
	void removeKeyframe(int frameIndex);

	std::vector<Light> light;

	std::map<int, Frame> keyframes;
	std::vector<Frame> frames;

	int currentFrame{ -1 };
	int totalFrames{ 0 };

	glm::vec3 positioOnTerrain;

	std::shared_ptr<ts::Texture2D> screen_texture;
	std::shared_ptr<ts::SpriteAnimation> sprite;
	std::shared_ptr<ts::ComputeShader> raytraceCS;

	void applyRayTracing(Frame& frame);
};