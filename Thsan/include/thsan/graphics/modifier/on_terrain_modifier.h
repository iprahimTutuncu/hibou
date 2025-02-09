#pragma once
#include <thsan/graphics/modifier.h>
#include <thsan/graphics/terrain/terrain.h>
#include <thsan/graphics/compute_shader.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace Thsan {
	struct TerrainOptions;
	struct WindowOption;

	class SpriteAnimation;
	class View;

	class THSAN_API OnTerrainModifier: public Modifier
	{
	public:
		OnTerrainModifier(int terrain_size): terrain_size(terrain_size) {};
		virtual void setTerrainOptions(TerrainOptions desc, WindowOption windowOptions) = 0;
		virtual void setView(std::shared_ptr<View> view) = 0;

		/*
			potentielle refacto: spriteAnimation contient deja une position 3D, un scale et une rotation, autant les utilisers
			voir quoi faire pour        

			note: la position est utiliser pour positioner la fake 3D en 2D, pas possible
			
			float curr_height; //questionnable
            float line_begin; //retirable
            float line_end; //retirable
            float dx;
            float sprite_z;
		*/
		virtual void add(std::shared_ptr<SpriteAnimation> spriteAnimation) = 0;
		virtual void update() = 0;

		static std::shared_ptr<OnTerrainModifier> create(int terrain_size);

	private:
        struct SpriteData {

            SpriteAnimation* spriteAnimation;
            glm::vec2 scale;
            glm::vec3 position;
            float rotation;
            float curr_height;
            float line_begin;
            float line_end;
            float dx;
            float sprite_z;
        };
	protected:
		int terrain_size;
	};

	class OnTerrainModifierImpl : public OnTerrainModifier
	{
	public:
		OnTerrainModifierImpl(int terrain_size);
		void setTerrainOptions(TerrainOptions desc, WindowOption windowOptions) override;
		void setView(std::shared_ptr<View> view) override;

		void add(std::shared_ptr<SpriteAnimation> spriteAnimation) override;
		void update() override;
	private:
		struct SpriteData {

			std::shared_ptr<SpriteAnimation> spriteAnimation;
			glm::vec2 scale;
			glm::vec3 position;
			float rotation;
			float curr_height;
			float line_begin;
			float line_end;
			float dx;
			float sprite_z;
		};

		TerrainOptions terrainDesc;
		float screenWidth;

		std::vector<SpriteData*> spritesData;
		std::vector<std::shared_ptr<SpriteAnimation>> sprites;
		std::shared_ptr<View> pView;
	};
}