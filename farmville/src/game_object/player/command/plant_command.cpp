#include "plant_command.h"
#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>
#include <core/game_object/data/data.h>
#include <core/game_object/game_object.h>
#include <core/scene.h>

PlantCommand::PlantCommand()
{
}

void PlantCommand::execute(GameObject& parent, Scene& scene, const float& dt)
{
	//make sure the animationis set to be played
	if (!parent.hasComponent(DataType::PLAYER)) {
		data = new PlayerData();
		parent.setComponent(DataType::PLAYER, data);
	}
	else {
		data = parent.getComponent<PlayerData>(DataType::PLAYER);
	}

	if (data->curr_state == PlayerData::State::idle)
		data->curr_state = PlayerData::State::plant;
	//remove the seed from existence

	parent.addOnSpriteAnimationRenderEvent("plant", 0.8f, true, [&](GameObject& go, Scene& scene, const float& at) {
		plantFactory = new PlantFactory(&scene);
		GameObject* plant = plantFactory->createTrilleRouge();
		PlantData* plant_data = plant->getComponent<PlantData>(DataType::PLANT);

		Transform* tr_plant = plant->getComponent<Transform>(DataType::TRANSFORM);
		Transform* tr_player = parent.getComponent<Transform>(DataType::TRANSFORM);

		plant_data->current_state = PlantData::State::planted;

		tr_plant->position = tr_player->position;
		tr_plant->scale = glm::vec2(0.25f, 0.25f);
	});
	
}

void PlantCommand::undo(GameObject& parent, Scene& scene, const float& dt)
{
}
