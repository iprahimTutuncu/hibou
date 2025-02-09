#pragma once
#include "graphics/renderer/renderer.h"

namespace Thsan 
{
	struct THSAN_API WindowOption
	{
		int screenWidth{};
		int screenHeight{};
	};

	struct THSAN_API TerrainOptions
	{
		float position[3];
		float normalBlurr[2];

		int terrainSize{};

		int normalBlurIteration{};
		float LOD{};
		float LODStartAt{};
		float angle{};
		float horizon{};
		float scaleHeight{};
		float spriteRenderDistance{};
		float terrainRenderDistance{};
		float shininess{};
		int blurType{};

		char albedoTexturePath[256]{};
		char heightTexturePath[256]{};
		char collisionHeightTexturePath[256]{};
		char collisionMaskTexturePath[256]{};
	};

	struct THSAN_API RenderOptions
	{
		Thsan::DebugRender debugRender{ Thsan::DebugRender::None};
		Thsan::TerrainOptions terrainOptions;
		bool enableRendererTerrain{ true };
	};

	struct THSAN_API Options
	{
		RenderOptions renderOptions;
		WindowOption windowOptions;
	};
}