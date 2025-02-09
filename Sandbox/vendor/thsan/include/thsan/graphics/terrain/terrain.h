#pragma once
#include "../../options.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace tsm
{
	class Color;
}

namespace Thsan {
	class Texture2D;


	class THSAN_API Terrain {
	public:

		Terrain() = default;
		virtual ~Terrain() = default;

		static std::shared_ptr<Terrain> create();

		virtual void init(TerrainOptions options, WindowOption windowOption) = 0;

		virtual float getRenderHeight(glm::vec2 position) = 0;
		virtual float getPhysicalHeight(glm::vec2 position) = 0;
		virtual float getCollisionMask(glm::vec2 position) = 0;
		virtual glm::vec3 getNormal(glm::vec2 position) = 0;
		virtual tsm::Color getAlbedo(glm::vec2 position) = 0;

		virtual void update(Thsan::TerrainOptions data, Thsan::WindowOption windowOption) = 0;
		virtual TerrainOptions getTerrainOptions() = 0;
		virtual std::shared_ptr<Texture2D> getRenderedDepthTexture() = 0;
		virtual std::shared_ptr<Texture2D> getRenderedAlbedoTexture() = 0;
		virtual std::shared_ptr<Texture2D> getRenderedPositionTexture() = 0;
		virtual std::shared_ptr<Texture2D> getRenderedNormalTexture() = 0;
	};
}