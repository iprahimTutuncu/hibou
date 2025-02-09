#pragma once
#include "thsan/core.h"
#include <memory>
#include <vector>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_float3x3.hpp>
#include <unordered_map>

namespace tsm{
	class AbstractCamera;
	class Transform;
}

namespace Thsan {
	class Shader;
	class Texture2D;
	class View;

	enum class Mode
	{
		fill,
		wired
	};

	enum class Debug
	{
		none,
		normal,
		position,
		diffuse,
		specular,
		ambiant,
		shadow
	};

	namespace Blend
	{
		enum class Factor
		{
			zero,
			one,
			sourceColor, oneMinusSourceColor,
			destinationColor, oneMinusDestinationColor,
			sourceAlpha, oneMinusSourceAlpha,
			destinationAlpha, oneMinusDestinationAlpha
		};

		enum class Equation
		{
			add,
			subtract,
			reverseSubtract,
			min,
			max
		};
	}

	struct BlendMode
	{
		bool enable;
		Blend::Factor sourceFactor;
		Blend::Factor destionationFactor;
		Blend::Equation equation;
	};

	enum class DepthFunc
	{
		less,
		lessEqual,
		greater,
		greaterEqual,
		equal,
		notEqual,
		always,
		never
	};

	enum class TextureType {
		Default,
		DepthBuffer,
		Position,
		Normal
	};


	enum class RendererType {
		rendererTerrain,
		renderer2D
	};

	class THSAN_API RenderStates {
	public:
		virtual inline std::weak_ptr<Shader> getShader() const = 0;
		virtual inline void setShader(std::weak_ptr<Shader> shader) = 0;
		virtual inline void setBlend(BlendMode blendMode) = 0;

		virtual inline void setTransform(std::weak_ptr<tsm::Transform> transform) = 0;
		virtual inline void setTexture(TextureType type, std::weak_ptr<Texture2D> texture) = 0;
		virtual inline void unsetTexture(TextureType type) = 0;

		virtual inline void setView(std::weak_ptr<View> view) = 0;

		virtual RendererType getRendererType() = 0;
		static std::shared_ptr<RenderStates> create(RendererType type);

		virtual void bind() const = 0;
		virtual void unBind() const = 0;
	};

	class RenderStatesImpl : public RenderStates {
	public:
		RenderStatesImpl(RendererType type);

		// Inherited via RenderStates
		virtual inline std::weak_ptr<Shader> getShader() const override;

		virtual inline void setShader(std::weak_ptr<Shader> shader) override;

		virtual inline void setBlend(BlendMode blendMode) override;

		virtual inline void setTransform(std::weak_ptr<tsm::Transform> transform) override;

		virtual inline void setTexture(TextureType type, std::weak_ptr<Texture2D> texture) override;

		virtual inline void unsetTexture(TextureType type) override;


		virtual inline void setView(std::weak_ptr<View> view) override;

		virtual RendererType getRendererType() override;

		// Hérité via RenderStates
		virtual void bind() const override;
		virtual void unBind() const override;


	private:
		int screenWidth{ 0 };
		int screenHeight{ 0 };
		std::weak_ptr<Shader> shader;
		std::unordered_map<TextureType, std::weak_ptr<Texture2D>> textureMap;
		std::weak_ptr<View> view;
		std::weak_ptr<tsm::Transform> transform;

		RendererType rendererType;

		BlendMode blendMode{ true, Blend::Factor::sourceColor, Blend::Factor::oneMinusSourceColor, Blend::Equation::add };
		Debug debug{ Debug::none };
	};

}